#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Query.h>
#include <znc/Chan.h>

#include <sstream>
#include <vector>
#include <queue>

//TODO
#include <fstream>
# define log(x) { std::ofstream m("/Users/zwimer/Desktop/log.txt", std::ios::app); m << __PRETTY_FUNCTION__ << " - " << __LINE__ << ": " << x << std::endl; m.close();}

/************************************************************/
/*															*/
/*  				  Forward Declarations  			   	*/
/*															*/
/************************************************************/


// Don't pollute the global namespace
// SMH = Self Msg Helpers
namespace SMH {

	// A message class
	struct Msg;

	// Comparators
	struct MsgCompare;

	// For clarity: MsgLine
	typedef std::priority_queue <
		Msg,
		std::vector<Msg>,
		MsgCompare
	> MsgList;

};

/************************************************************/
/*															*/
/*						Helper classes						*/
/*															*/
/************************************************************/


// Represents a message
struct SMH::Msg {

	// Constructors and destructor
	Msg(const timeval& whn, const CString wht) : when(whn), what(wht) {}
	Msg(const timeval& whn, const CString wht, const CString & fmt) : 
		when(whn), what(wht), format(fmt) {}
	~Msg() {}

	// Assignment operator
	Msg & operator= ( const Msg & m ) {
		when.tv_usec = m.when.tv_usec;
		when.tv_sec = m.when.tv_sec;
		format = m.format;
		what = m.what;
		return * this;
	}

	// Representation
	timeval when;
	CString what;
	CString format;
};

// Define a comparator for Msg
struct SMH::MsgCompare {
	bool operator() (const Msg& a, const Msg& b) {
//TODO: 0 comes last
		return timercmp( & a.when, & b.when, > );
	}
};

//TODO: const cast

//TODO: remove
std::ostream & operator << (std::ostream & str, const SMH::Msg s) {
	str << "Msg( " << s.when.tv_sec << ", " << s.what << ", " << s.format;
	return str;
}  

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
	std::map< CString, SMH::MsgList > sent;
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
	sent[who].push( SMH::Msg(v , sMessage) );

	//TODO:
	std::stringstream s; s << who << " sent " << sMessage << '\n';
	log(s.str());

	// Nothing bad happened
	return CONTINUE;
}

// Intercept the buffer being sent to the user
SelfMsg::EModRet SelfMsg::OnChanBufferStarting( CChan& ch, CClient & cli ) {

	// Get a mutable version of the buffer
	CBuffer * buf = (CBuffer*) & ch.GetBuffer();

	// Get user's hostmask, and create a format from it
	CString format = ch.FindNick(cli.GetNick())->GetHostMask();

	//TODO:
	std::stringstream s; 
	s << "Startup: Intercept buf from " << ch.GetName() << '\n';
	s << "\t - Nick = " << cli.GetNick() << '\n';
	s << "\t - HM = " << format << '\n';
	s << "Adding lines now:" << '\n';

	format = ":" + format + " PRIVMSG #rpisec :{text}";

	// The new buffer to be used;Make the list of sent messages the new buffer
	SMH::MsgList newBuf;

	// Update each line's who in sent, and add them all to newBuf
	for ( SMH::MsgList & mySent = sent[ch.GetName()] ; mySent.size(); mySent.pop() ) {
		((SMH::Msg*) (&mySent.top()))->format = format;
		newBuf.push(mySent.top());
		s << "\t\t Added (sent): " << mySent.top() << '\n';
	}

	// Add each line in the buf to sent
	const int n = buf->GetLineCount();
	for ( int i = 0; i < n; i++ ) {
		const CBufLine & tmp = buf->GetBufLine( i );
		newBuf.push( SMH::Msg( tmp.GetTime(), tmp.GetText(), tmp.GetFormat() ) );
		s << "\t\t Added (buf): " << SMH::Msg( tmp.GetTime(), tmp.GetText(), tmp.GetFormat() )<< '\n';
	}

	// Clear the old buf
	buf->Clear();

	// Update buf with the entries in newBufLines
	for ( int i = 0; newBuf.size(); newBuf.pop() ) {
		const SMH::Msg & nxt = newBuf.top();
		s << "\t\t\t NEW: " << nxt << '\n';
		buf->AddLine( nxt.format, nxt.what );
		((CBufLine*) &(buf->GetBufLine(i++)))->SetTime( nxt.when );
	}
	
	log(s.str());

	// Clear the (now old) sent buffer
	SMH::MsgList & m = sent[ch.GetName()];
	while (m.size()) m.pop();

	// Nothing bad happened
	return CONTINUE;
}

// Versions 1.7+
// SelfMsg::EModRet SelfMsg::OnPrivBufferStarting(CQuery& Query, CClient& Client) { }


/************************************************************/
/*															*/
/*						Registration						*/
/*															*/
/************************************************************/


// 'Register' this mod as a mod
MODULEDEFS(SelfMsg, "Change a public buf")
