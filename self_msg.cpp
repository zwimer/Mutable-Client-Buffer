#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Query.h>
#include <znc/Chan.h>

#include <priority_queue>
#include <sstream>
#include <vector>


/************************************************************/
/*															*/
/*  				  Forward Declarations  			   	*/
/*															*/
/************************************************************/


// Don't pollute the global namespace
namespace SelfMsgHelpers {

	// Represents a message
	struct MsgCompare;
	struct Msg;

	// Compares BufLines 
	struct BufLineCompare

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

	// Intercept the buffer being sent to the user
	EModRet OnChanBufferStarting( CChan& ch, CClient & cli ) override;

	//Read all messages the user sends
	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override;

	// Versions 1.7+
	// EModRet OnPrivBufferStarting(CQuery& Query, CClient& Client) override;

private:

	// Store what messages the user sent to who
	std::map< CString, SelfMsgHelper::MsgList > sent;
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
bool SelfMsg::OnBoot() override {
	// This is called when the app starts up (only modules that are loaded
	// in the config will get this event)
	return true;
}

// Intercept the buffer being sent to the user
EModRet SelfMsg::OnChanBufferStarting( CChan& ch, CClient & cli ) override {

#if 0
	// The new format we will be using below
	static const CString cs(":zwimer!zwimer@net-pd2sh0.res."
							"rr.com PRIVMSG #rpisec :{text}");

	// Get a mutable version of the buffer
	CBuffer * buf = (CBuffer*) & ch.GetBuffer();

	// Record the contents of the buffer to a file
	std::ofstream f("/Users/zwimer/Desktop/testing_out_altered.txt");
	for( unsigned int i = 0; i < buf->GetLineCount(); ++i ) {

		// Record the text, unix time stamp, format it was sent in
		f << "\nText: " << buf->GetBufLine(i).GetText()
		  << "\nTime: " << buf->GetBufLine(i).GetTime().tv_sec 
		  << "\nFormat: " << buf->GetBufLine(i).GetFormat();

		// If time != 0 (real post), change the format of the current line
		if (buf->GetBufLine(i).GetTime().tv_sec) {
			((CBufLine*) & buf->GetBufLine(i))->SetFormat( cs );
		}

		// Record the format
		f << "\nNew Format: " << buf->GetBufLine(i).GetFormat() << '\n';
	}
#endif

	// Nothing bad happened
	return CONTINUE;
}

//Read all messages the user sends
EModRet SelfMsg::OnUserMsg(CString& who, CString& sMessage) override {
	
	// Record the time
	timeval v; gettimeofday( &v, nullptr ); 

	// Record what was sent where and when
	sent[who].push_back( v , sMessage );

	// Nothing bad happened
	return CONTINUE;
}

// Versions 1.7+
// EModRet SelfMsg::OnPrivBufferStarting(CQuery& Query, CClient& Client) override;


// 'Register' this mod as a mod
MODULEDEFS(SelfMsg, "Change a public buf")


/************************************************************/
/*															*/
/*						Helper classes						*/
/*															*/
/************************************************************/


// Represents a message
struct SelfMsgHelper::Msg {

	// Constructors and destructor
	Msg(const timeval& whn, const CString wht) : when(whn), what(wht) {}
	~Msg() {}

	// Representation
	const CString what;
	const timeval when;
};

// Define a comparator for Msg
struct SelfMsgHelper::MsgCompare {
	bool operator() (const Msg& a, const Msg& b) {
		return timercmp( a.when, b.when, < );
	}
};

// Compares BufLines 
struct SelfMsgHelper::BufLineCompare {
	bool operator() (const CBufLine& a, const CBufLine & b) {
		return timercmp( a.GetTime(), b.GetTime(), < );
	}
};


/************************************************************/
/*															*/
/*						Registration						*/
/*															*/
/************************************************************/


// 'Register' this mod as a mod
MODULEDEFS(SelfMsg, "Change a public buf")
