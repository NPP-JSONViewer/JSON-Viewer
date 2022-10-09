// dllmain.cpp : Defines the entry point for the DLL application.

#include "NppJsonPlugin.h"

NppJsonPlugin g_NppJsonPlugin;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID /*lpReserved*/)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_NppJsonPlugin.PluginInit(hModule);
        break;

    case DLL_PROCESS_DETACH:
        g_NppJsonPlugin.PluginCleanup();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

// Below are the mandatory function to be implemented as Notepad++ requires them

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
    g_NppJsonPlugin.SetInfo(notpadPlusData);
}

extern "C" __declspec(dllexport) const TCHAR *getName()
{
    return g_NppJsonPlugin.GetPluginName();
}

extern "C" __declspec(dllexport) FuncItem *getFuncsArray(int *nbF)
{
    return g_NppJsonPlugin.GetFuncsArray(nbF);
}

extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
    g_NppJsonPlugin.ProcessNotification(notifyCode);
}

// Here you can process the Npp Messages
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_NppJsonPlugin.MessageProc(msg, wParam, lParam);
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return g_NppJsonPlugin.IsUnicode();
}
#endif    // UNICODE
