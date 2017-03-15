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

    //When a user sends a message, colors it red and appends "in France" to it.
   	EModRet OnUserMsg(CString& sTarget, CString& sMessage) override {
        PutModule("[" + sTarget + "] usermsg [" + sMessage + "]");
        sMessage = "\0034" + sMessage + " in France\003";
        return CONTINUE;
	  }
	
    //When a user receives a message, bolds it and appends "in France" to it.
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
    Info.SetArgsHelpText("Color code help text");
}

MODULEDEFS(CSampleMod, "Color code?")
