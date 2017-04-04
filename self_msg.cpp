#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Query.h>
#include <znc/Chan.h>

#include <sstream>
#include <vector>
#include <queue>


/************************************************************/
/*															*/
/*  				  Forward Declarations  			   	*/
/*															*/
/************************************************************/


// Don't pollute the global namespace
namespace SelfMsgHelpers {

	// A message class
	struct Msg;

	// Comparators
	struct BufLineCompare;
	struct MsgCompare;

	// For clarity: MsgLine
	typedef std::priority_queue <
		Msg,
		std::vector<Msg>,
		MsgCompare
	> MsgList;

	// For clarity: BufLineList
	typedef std::priority_queue <
		CBufLine,
		std::vector<CBufLine>,
		BufLineCompare
	> BufLineList;

};

/************************************************************/
/*															*/
/*						Helper classes						*/
/*															*/
/************************************************************/


// Represents a message
struct SelfMsgHelpers::Msg {

	// Constructors and destructor
	Msg(const timeval& whn, const CString wht) : when(whn), what(wht) {}
	Msg(const timeval& whn, const CString wht, const CString & fmt) : 
		when(whn), what(wht), format(fmt) {}
	~Msg() {}

	// Representation
	const timeval when;
	const CString what;
	CString format;
};

// Define a comparator for Msg
struct SelfMsgHelpers::MsgCompare {
	bool operator() (const Msg& a, const Msg& b) {
		return timercmp( & a.when, & b.when, < );
	}
};

// Compares BufLines 
struct SelfMsgHelpers::BufLineCompare {
	bool operator() (const CBufLine& a, const CBufLine & b) {
		timeval atr = a.GetTime(), btr = b.GetTime();
		return timercmp( & atr, & btr, < );
	}
};


/************************************************************/
/*															*/
/*  					SelfMsg Header						*/
/*															*/
/************************************************************/


// Our module
class SelfMsg : public CModule {
public:

	//Constructor and destructor
	MODCONSTRUCTOR(SelfMsg) {}
	~SelfMsg() override { PutModule("I'm being unloaded!"); }

	//When the system is booted up (nothing needs to happen)
	bool OnBoot() override;

	//Read all messages the user sends
	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override;

	// Intercept the buffer being sent to the user
	EModRet OnChanBufferStarting( CChan& ch, CClient & cli ) override;

	// Versions 1.7+
	// EModRet OnPrivBufferStarting(CQuery& Query, CClient& Client) override;

private:

	// Store what messages the user sent to who
	std::map< CString, SelfMsgHelpers::MsgList > sent;
};


/************************************************************/
/*															*/
/*  				   Web Client Info						*/
/*															*/
/************************************************************/


// Information for the web client
template <> void TModInfo<SelfMsg>(CModInfo& Info) {
//TODO
	Info.SetWikiPage("My wiki page");
	Info.SetHasArgs(false);
	// Info.SetArgsHelpText("Args help text"); // Not needed, no args
}


/************************************************************/
/*															*/
/*  				    SelfMsg Functions					*/
/*															*/
/************************************************************/


//When the system is booted up (nothing needs to happen)
bool SelfMsg::OnBoot() {
	// This is called when the app starts up (only modules that are loaded
	// in the config will get this event)
	return true;
}

//Read all messages the user sends
SelfMsg::EModRet SelfMsg::OnUserMsg(CString& who, CString& sMessage) {
	
	// Record the time
	timeval v; gettimeofday( &v, nullptr ); 

	// Record what was sent where and when
	sent[who].push( SelfMsgHelpers::Msg(v , sMessage) );

	// Nothing bad happened
	return CONTINUE;
}

// Intercept the buffer being sent to the user
SelfMsg::EModRet SelfMsg::OnChanBufferStarting( CChan& ch, CClient & cli ) {

	// New buffer's lines
	SelfMsgHelpers::BufLineList newBufLines;

	// Get a mutable version of the buffer
	CBuffer * buf = (CBuffer*) & ch.GetBuffer();

	// Get user's hostmask, and create a format from it
	CString format = ch.FindNick(cli.GetNick())->GetHostMask();
	format += " PRIVMSG #rpisec :{text}";

	// Update each line's who in sent
	for ( SelfMsgHelpers::Msg & i : sent[ch.GetName()] ) {
		i.format = format;
	}

	// Add each line in the buf to sent
	const int n = buf->GetLineCount();
	for ( int i = 0; i < n; i++ ) {
		const CBufLine & tmp = buf->GetBufLine( i );
		sent[ch.GetName()].push( SelfMsgHelpers::Msg( 
									tmp.GetTime(), 
									tmp.GetText(), 
									tmp.GetFormat() )
		);
	}

	// Update buf with the entries in newBufLines
	for ( buf->Clear(); newBufLines.size(); newBufLines.pop() ) {
		const Msg & nxt = newBufLines.top();
		buf->AddLine( nxt.format, nxt.what );
		((CBufLine*) &(buf->GetBufLine(buf->GetLineCount())))->SetTime( nxt.when );
	}
	
	// Clear the (now old) sent buffer
	sent[ch.GetName()].clear();

	// Nothing bad happened
	return CONTINUE;
}

// Versions 1.7+
// SelfMsg::EModRet SelfMsg::OnPrivBufferStarting(CQuery& Query, CClient& Client) { }


// 'Register' this mod as a mod
MODULEDEFS(SelfMsg, "Change a public buf")


/************************************************************/
/*															*/
/*						Registration						*/
/*															*/
/************************************************************/


// 'Register' this mod as a mod
MODULEDEFS(SelfMsg, "Change a public buf")
