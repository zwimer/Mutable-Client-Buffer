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
		PutUser(":irc.znc.in 422 :MOTD Welcome friend! I am the M.O.T.D.");
	}

	//When the system is booted up (nothing needs to happen)
	bool OnBoot() override {
		// This is called when the app starts up (only modules that are loaded
		// in the config will get this event)
		return true;
	}

	//Intercept the buffer being sent to the user
	EModRet OnChanBufferStarting( CChan& ch, CClient & cli ) override {

		// Get a mutable version of the buffer
		CBuffer * buf = (CBuffer*) & (ch.GetBuffer());

		// Record the contents of the buffer
		std::stringstream s; s << buf; 

		// Save the contents to a file
		std::ofstream f("/Users/zwimer/Desktop/testing_out.txt");
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

	//Intercept the private buffers being sent to the user
	EModRet OnPrivBufferStarting( CQuery& ch, CClient& cli ) override {

		// Get a mutable version of the buffer
		CBuffer * buf = (CBuffer*) & (ch.GetBuffer());

		// Record the contents of the buffer
		std::stringstream s; s << buf; 

		// Save the contents to a file
		std::ofstream f("/Users/zwimer/Desktop/testing_priv_out.txt");
		f << s.str();
		f.close();

		//Nothing bad happened
		return CONTINUE;
	}

private:

};

// Information for the web client
template <> void TModInfo<CSampleMod>(CModInfo& Info) {
	Info.SetWikiPage("My wiki page");
	Info.SetHasArgs(false);
	// Info.SetArgsHelpText("Args help text"); // Not needed, no args
}

// 'Register' this mod as a mod
MODULEDEFS(CSampleMod, "Read buf to file")
