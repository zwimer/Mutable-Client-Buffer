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
	MODCONSTRUCTOR(CSampleMod) {}
	~CSampleMod() override { PutModule("I'm being unloaded!"); }

	//When the client logs in, set the M.O.T.D.
	void OnClientLogin() override {
		PutUser(":irc.znc.in 422 :MOTD Welcome friend! I am the M.O.T.D. for RefiendChangeFormat");
	}

	//When the system is booted up (nothing needs to happen)
	bool OnBoot() override {
		// This is called when the app starts up (only modules that are loaded
		// in the config will get this event)
		return true;
	}

	// Intercept the buffer being sent to the user
	EModRet OnChanBufferStarting( CChan& ch, CClient & cli ) override {

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

		// Nothing bad happened
		return CONTINUE;
	}

	//Read all messages the user sends
	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override {
		
		//Log who sent what where
		std::ofstream f("/Users/zwimer/Desktop/Sent_what_where.txt", std::ofstream::app);
		f << sTarget << " said " << sMessage << '\n';
		f.close();

		// Nothing bad happened
		return CONTINUE;
	}

	// Versions 1.7+
	// EModRet OnPrivBufferStarting(CQuery& Query, CClient& Client) override;
};

// Information for the web client
template <> void TModInfo<CSampleMod>(CModInfo& Info) {
	Info.SetWikiPage("My wiki page");
	Info.SetHasArgs(false);
	// Info.SetArgsHelpText("Args help text"); // Not needed, no args
}

// 'Register' this mod as a mod
MODULEDEFS(CSampleMod, "Change a public buf")
