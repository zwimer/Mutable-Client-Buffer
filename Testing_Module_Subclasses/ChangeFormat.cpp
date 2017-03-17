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
		PutUser(":irc.znc.in 422 :MOTD Welcome friend! I am the M.O.T.D. for PubBuf");
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
		const CBuffer & cbuf = ch.GetBuffer();
		CBuffer * buf = (CBuffer*) & cbuf;

		// Record the contents of the buffer
		std::stringstream s;
		for(unsigned int i = 0; i < buf->GetLineCount(); i++) {

			// Record the text
			s << "Text: " << buf->GetBufLine(i).GetText() << '\n';

			// Record the unix time stamp
			s << "Time: " << buf->GetBufLine(i).GetTime().tv_sec << '\n';

			// Record who and where the msg was
			s << "Format: " << buf->GetBufLine(i).GetFormat() << '\n';

			// If time != 0
			if (buf->GetBufLine(i).GetTime().tv_sec) {

				// Change the format
				const CString cs(":zwimer!zwimer@net-pd2sh0.res.rr.com PRIVMSG #rpisec :{text}");
				const CBufLine & line = buf->GetBufLine(i);
				CBufLine * linePtr = (CBufLine*) & line;
				linePtr->SetFormat( cs );
			}

			s << "New Format: " << buf->GetBufLine(i).GetFormat() << '\n' << '\n';
		}

		// Save the contents to a file
		std::ofstream f("/Users/zwimer/Desktop/testing_out_altered.txt");
		f << s.str();
		f.close();

		//Nothing bad happened
		return CONTINUE;
	}

	//Read all messages the user sends
	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override {
		
		//Log who sent what where
		std::stringstream s; s << sTarget << " said " << sMessage << '|';
		
		// Append what s.str() to the file below
		std::ofstream f("/Users/zwimer/Desktop/Sent_what_where.txt", std::ofstream::app);
		f << s.str(); 
		f.close();

		return CONTINUE;
	}

	// Versions 1.7+
	//EModRet OnPrivBufferStarting(CQuery& Query, CClient& Client) override { return CONTINUE; }

private:

};

// Information for the web client
template <> void TModInfo<CSampleMod>(CModInfo& Info) {
	Info.SetWikiPage("My wiki page");
	Info.SetHasArgs(false);
	// Info.SetArgsHelpText("Args help text"); // Not needed, no args
}

// 'Register' this mod as a mod
MODULEDEFS(CSampleMod, "Read public buf to file")
