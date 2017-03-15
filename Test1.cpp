#include <znc/Modules.h>

class CSampleMod : public CModule
{
	public:
   		MODCONSTRUCTOR(CSampleMod) {}
   	    ~CSampleMod() override { PutModule("I'm being unloaded!"); }

    bool OnBoot() override {
        // This is called when the app starts up (only modules that are loaded
        // in the config will get this event)
        return true;
    }

    //When a user sends a message, appends "in France" to it.
   	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override {
        PutModule("[" + sTarget + "] usermsg [" + sMessage + "]");
        sMessage = sMessage + " in France";
        return CONTINUE;
    }
};
template <>
void TModInfo<CSampleMod>(CModInfo& Info) {
    Info.SetWikiPage("Test1");
    Info.SetHasArgs(true);
    Info.SetArgsHelpText("Add In France help text");
}

MODULEDEFS(CSampleMod, "Add In France")
