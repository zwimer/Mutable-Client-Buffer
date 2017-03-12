#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Chan.h>

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

		// Log it
		PutModule("[" + sTarget + "] usermsg [" + sMessage + "]");

		// Color code it
		sMessage = "\0034" + sMessage + "\003";

		// Nothing bad happened
		return CONTINUE;
	}
	
	// When the user recieves a message
   	EModRet OnPrivMsg(CNick& Nick, CString& sMessage) override {

		// Log it
		PutModule("[" + Nick.GetNick() + "] usermsg [" + sMessage + "]");

		//Color code it
		sMessage = "\002" + sMessage + "\002";

		// Nothing bad happened
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
MODULEDEFS(CSampleMod, "Checking if color coding works")
