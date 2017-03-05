#include <znc/Modules.h>
#include <znc/Client.h>
#include <znc/Chan.h>

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
        sMessage = "\0034" + sMessage + " in France\003";
        return CONTINUE;
	}
	
   	EModRet OnPrivMsg(CNick& Nick, CString& sMessage) override {
        PutModule("[" + Nick.GetNick() + "] usermsg [" + sMessage + "]");
        sMessage = "\002" + sMessage + " in France\002";
        return CONTINUE;
    }
};
template <>
void TModInfo<CSampleMod>(CModInfo& Info) {
    Info.SetWikiPage("Test2");
    Info.SetHasArgs(true);
    Info.SetArgsHelpText("Color code?");
}

MODULEDEFS(CSampleMod, "To be used as a sample for writing modules")
