#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Query.h>
#include <znc/Chan.h>

#include <queue>


// Debug mode
#ifdef DEBUG_MODE

	#include <sstream>
	#include <fstream>

	// Log a message
	#define log(x) { auto l = __LINE__; \
		std::ofstream m("/Users/zwimer/Desktop/log.txt", std::ios::app); \
		m << __PRETTY_FUNCTION__ << " - " << l << ": " << x << std::endl; \
		m.close(); \
	}

#endif


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


//! Represents a message
/*! This struct contains a message, its format, and when it was sent */
struct SMH::Msg {

	//! Constructor
	/*! This constructor takes in a timeval and message and creates the Msg from them. This constructor leaves the format blank */
	Msg(const timeval& whn, const CString wht) : when(whn), what(wht), format("") {}

	//! Constructor 
	/* This constructor takes in a timeval, a message, and a format, then creates the Msg from them */
	Msg(const timeval& whn, const CString wht, const CString & fmt) : 
		when(whn), what(wht), format(fmt) {}
	

	// Representation

	//! When the message was sent
	timeval when;
	//! The message
	CString what;
	//! The format of the message
	CString format;
};

//! A comparator for Msg
struct SMH::MsgCompare {

	//! Compares two messages
	/*! Returns true if all bits in a are 0, false if all bits in b are 0. Otherwise, return a.when > b.when */
	bool operator() (const Msg& a, const Msg& b) {

		// Zero Msgs should always be on the bottom of the queue
		if (!a.when.tv_sec && !a.when.tv_usec && !a.what.size() && !a.format.size())
			return true;
		else if (!b.when.tv_sec && !b.when.tv_usec && !b.what.size() && !b.format.size())
			return false;

		// Otherwise return a.when > b.when
		return timercmp( & a.when, & b.when, > );
	}
};

// Debug mode
#ifdef DEBUG_MODE

	// Print a Msg
	std::ostream & operator << (std::ostream & str, const SMH::Msg s) {
		str << "Msg( " << s.when.tv_sec << ", " << s.what << ", " << s.format;
		return str;
	}  

#endif


/************************************************************/
/*															*/
/*  					SelfMsg Header						*/
/*															*/
/************************************************************/


//! A znc module that allows one to receive their own messages.
/*!
	This module will record all messages the user sends. When the user next connects to znc and requests their buffer, this module will add the user's messages to the buffer they receive.
*/
class SelfMsg : public CModule {
public:

	//! Constructor
	/*! The normal mod constructor */
	MODCONSTRUCTOR(SelfMsg) {}

	//! Destructor
	/*! Inform the user that this module has been unloaded */	
	~SelfMsg() override { PutModule("The selfMsg module is being unloaded!"); }

	//! Called when znc is first turned on
	/*! Does nothing but override Module's on boot function */
	bool OnBoot() override;

	//! Read every message the user sends
	/*! This function is called whenever the user sends a message. This function records each messages, relevant information, then stores it in the 'sent' map */
	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override;

	//! Alter the buffer being sent to the user
	/*! This function intercepts the buffer being sent to the user. It then inserts each message the user sent into the buffer and finally clears the 'sent' map. */
	EModRet OnChanBufferStarting( CChan& ch, CClient & cli ) override;

	// Versions 1.7+
	// EModRet OnPrivBufferStarting(CQuery& Query, CClient& Client) override;

private:

	// Store what messages the user sent to who
	/*! This map has CString key denoting who each message was sent to. The value in the map is a list of messages sorted by when each message was sent. This map will hold all messages that must be added to the buffer when it is next intercepted. */
	std::map< CString, SMH::MsgList > sent;
};


/************************************************************/
/*															*/
/*  				   Web Client Info						*/
/*															*/
/************************************************************/


//! Information for the web client
/*! This function takes in no arguments */
template <> void TModInfo<SelfMsg>(CModInfo& Info) {
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
	
	// Record the Msg and all needed info
	timeval v; gettimeofday( &v, nullptr ); 
	sent[who].push( SMH::Msg(v , sMessage) );

	// Nothing bad happened
	return CONTINUE;
}

// Intercept the buffer being sent to the user
SelfMsg::EModRet SelfMsg::OnChanBufferStarting( CChan& ch, CClient & cli ) {

	// Get a mutable version of the buffer
	CBuffer * buf = (CBuffer*) & ch.GetBuffer();

	// Get user's hostmask, and create a format from it
	CString format = ch.FindNick(cli.GetNick())->GetHostMask();
	format = ":" + format + " PRIVMSG " + ch.GetName() + " :{text}";

	// The new buffer to be used
	SMH::MsgList newBuf;

	// Update each line's who in sent, and add them all to newBuf
	for ( SMH::MsgList & mySent = sent[ch.GetName()] ; mySent.size(); mySent.pop() ) {
		((SMH::Msg*) (&mySent.top()))->format = format;
		newBuf.push(mySent.top());
	}

	// Add each line in the buf to sent
	const int n = buf->GetLineCount();
	for ( int i = 0; i < n; i++ ) {
		const CBufLine & tmp = buf->GetBufLine( i );
		newBuf.push( SMH::Msg( tmp.GetTime(), tmp.GetText(), tmp.GetFormat() ) );
	}

	// Clear the old buf
	buf->Clear();

	// Update buf with the entries in newBufLines
	for ( int i = 0; newBuf.size(); newBuf.pop() ) {
		const SMH::Msg & nxt = newBuf.top();
		buf->AddLine( nxt.format, nxt.what );
		((CBufLine*) &(buf->GetBufLine(i++)))->SetTime( nxt.when );
	}
	
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
MODULEDEFS(SelfMsg, "Record your own messages")
