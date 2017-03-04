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

   	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override {
        PutModule("[" + sTarget + "] usermsg [" + sMessage + "]");
        sMessage = sMessage + " in France";
        return CONTINUE;
    }
};
template <>
void TModInfo<CSampleMod>(CModInfo& Info) {
    Info.SetWikiPage("sample");
    Info.SetHasArgs(true);
    Info.SetArgsHelpText("Description of module arguments goes here.");
}

MODULEDEFS(CSampleMod, "To be used as a sample for writing modules")
