#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Chan.h>

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
};

// Information for the web client
template <> void TModInfo<CSampleMod>(CModInfo& Info) {
    Info.SetWikiPage("My wiki page");
    Info.SetHasArgs(false);
    // Info.SetArgsHelpText("Args help text"); // Not needed, no args
}

// 'Register' this mod as a mod
MODULEDEFS(CSampleMod, "Checking if color coding works")
