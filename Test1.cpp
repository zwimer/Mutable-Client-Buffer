#include <znc/Modules.h>

// Our module
class CSampleMod : public CModule {
public:

	// Constructor and destructor
	MODCONSTRUCTOR(CSampleMod) {}
	~CSampleMod() override { PutModule("I'm being unloaded!"); }

	// When the system is booted up (nothing needs to happen)
    bool OnBoot() override {
        // This is called when the app starts up (only modules that are loaded
        // in the config will get this event)
        return true;
    }

	// When the user sends a message
   	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override {

		// Log what who sent what message is
        PutModule("[" + sTarget + "] usermsg [" + sMessage + "]");

		// Append ' in France' to the message
        sMessage = sMessage + " in France";

		// Everything worked
        return CONTINUE;
    }
};

// Information for the web client
template <> void TModInfo<CSampleMod>(CModInfo& Info) {
    Info.SetWikiPage("My wiki page");
    Info.SetHasArgs(false);
    // Info.SetArgsHelpText("Args help text"); // Not needed, no args
}

// 'Register' this mod as a mod
MODULEDEFS(CSampleMod, "Add In France")
