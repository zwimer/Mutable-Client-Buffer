#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Query.h>
#include <znc/Chan.h>

#include <string>
#include <sstream>
#include <fstream>

class CSampleMod : public CModule
{
	public:
   		MODCONSTRUCTOR(CSampleMod) {}
   	    ~CSampleMod() override { PutModule("I'm being unloaded!"); }

	void OnClientLogin() override {
		PutUser(":irc.znc.in 422 :MOTD Wassup brah, you've connected to us, bro. Time to get your bro-seff on, bro.");
	}

    bool OnBoot() override {
        // This is called when the app starts up (only modules that are loaded
        // in the config will get this event)
        return true;
    }

	//Intercept the buffer being sent to the user
	EModRet OnChanBufferStarting( CChan& ch, CClient & cli ) override {

		//Get a mutable version of the buffer
		CBuffer * buf = (CBuffer*) & (ch.GetBuffer());

		std::string s; std::stringstream s2;
		s2 << buf; s = s2.str();		

		std::ofstream f("/Users/zwimer/Desktop/testing_out.txt");
		f << s;
		f.close();

		return CONTINUE;
	}

	//Read all messages the user sends
	EModRet OnUserMsg(CString& sTarget, CString& sMessage) {
		
		//Determine ones sent to the general chat
		std::stringstream s;
		s << sTarget << " said " << sMessage << '|';
		
		std::ofstream f("/Users/zwimer/Desktop/Sent_what_where.txt", std::ofstream::app);
		f << s.str(); f.close();

	}

	//EModRet OnPrivBufferStarting(CQuery& Query, CClient& Client) override { return CONTINUE; }

private:

	//Representation
	//std::map<CString, 

};
template <>
void TModInfo<CSampleMod>(CModInfo& Info) {
    Info.SetWikiPage("Test2");
    Info.SetHasArgs(true);
    Info.SetArgsHelpText("On Loging help text");
}

MODULEDEFS(CSampleMod, "On Login desc")
