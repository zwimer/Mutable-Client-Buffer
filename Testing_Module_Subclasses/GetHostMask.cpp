#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Query.h>
#include <znc/Chan.h>

#include <string>
#include <sstream>
#include <fstream>

// Our module
class CSampleMod : public CModule {
public:

	//Constructor and destructor
	MODCONSTRUCTOR(CSampleMod) { gettimeofday(&tv, NULL); }
	~CSampleMod() override { PutModule("I'm being unloaded!"); }

	//When the client logs in, set the M.O.T.D.
	void OnClientLogin() override {
		PutUser(":irc.znc.in 422 :MOTD Welcome friend! I am the M.O.T.D. for GetHostMask");
	}

	//When the system is booted up (nothing needs to happen)
	bool OnBoot() override {
		// This is called when the app starts up (only modules that are loaded
		// in the config will get this event)
		return true;
	}

	// Intercept the buffer being sent to the user
	EModRet OnChanBufferStarting( CChan& ch, CClient & cli ) override {

		// Get a mutable version of the buffer
		CBuffer * buf = (CBuffer*) & ch.GetBuffer();

		// Record the contents of the buffer to a file
		std::ofstream f("/Users/zwimer/Desktop/AddLine_out_1.txt");
		for( unsigned int i = 0; i < buf->GetLineCount(); ++i ) {
			f << "\nText: " << buf->GetBufLine(i).GetText()
			  << "\nTime: " << buf->GetBufLine(i).GetTime().tv_sec 
			  << "\nFormat: " << buf->GetBufLine(i).GetFormat();
		}

		// Adding "<- This is when I was loaded" to the end of the buffer
		const CString tmp("zwimer");
		CString cs(":");
		cs += ch.FindNick(tmp)->GetHostMask();
		cs += " PRIVMSG #rpisec :{text}"; 
		const CString str("<- This is when I was loaded");
		buf->AddLine( cs, str, & tv );

		// Record the contents of the buffer to a file
		std::ofstream f2("/Users/zwimer/Desktop/AddLine_out_1.txt");
		for( unsigned int i = 0; i < buf->GetLineCount(); ++i ) {
			f2 << "\nText: " << buf->GetBufLine(i).GetText()
			  << "\nTime: " << buf->GetBufLine(i).GetTime().tv_sec 
			  << "\nFormat: " << buf->GetBufLine(i).GetFormat();
		}

		// Nothing bad happened
		return CONTINUE;
	}

	// Versions 1.7+
	// EModRet OnPrivBufferStarting(CQuery& Query, CClient& Client) override;
private:

    struct timeval tv;

};

// Information for the web client
template <> void TModInfo<CSampleMod>(CModInfo& Info) {
	Info.SetWikiPage("My wiki page");
	Info.SetHasArgs(false);
	// Info.SetArgsHelpText("Args help text"); // Not needed, no args
}

// 'Register' this mod as a mod
MODULEDEFS(CSampleMod, "Get Hostmask")
