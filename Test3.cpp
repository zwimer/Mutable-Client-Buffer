#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Chan.h>

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

};
template <>
void TModInfo<CSampleMod>(CModInfo& Info) {
    Info.SetWikiPage("Test2");
    Info.SetHasArgs(true);
    Info.SetArgsHelpText("On Loging help text");
}

MODULEDEFS(CSampleMod, "On Login desc")
