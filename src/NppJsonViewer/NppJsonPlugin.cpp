#include "NppJsonPlugin.h"
#include "resource.h"
#include "Profile.h"
#include <tchar.h>

NppJsonPlugin* NppJsonPlugin::Callback::m_pNppJsonPlugin = nullptr;

NppJsonPlugin::NppJsonPlugin()
    : m_shortcutCommands(nTotalCommandCount)
{
    NppJsonPlugin::Callback::m_pNppJsonPlugin = this;
}

void NppJsonPlugin::PluginInit(HMODULE hModule)
{
    m_hModule = hModule;
}

void NppJsonPlugin::PluginCleanup() {}

void NppJsonPlugin::SetInfo(const NppData& nppData)
{
    m_NppData = nppData;
    InitCommandMenu();
    InitToolbarIcon();
    InitConfigPath();
}

const TCHAR* NppJsonPlugin::GetPluginName() const
{
    return PLUGIN_NAME;
}

FuncItem* NppJsonPlugin::GetFuncsArray(int* nbF)
{
    *nbF = nTotalCommandCount;
    return m_shortcutCommands.GetFuncItem();
}

void NppJsonPlugin::ProcessNotification(const SCNotification* notifyCode)
{
    switch (notifyCode->nmhdr.code)
    {
    case NPPN_TBMODIFICATION:
    {
        SetMenuIcon();
        break;
    }

    case NPPN_SHUTDOWN:
    {
        PluginCleanup();
        break;
    }

    case NPPN_BUFFERACTIVATED:
    {
        if (m_pJsonViewDlg && m_bNppReady && !m_bAboutToClose)
        {
            m_pJsonViewDlg->HandleTabActivated();
        }
        break;
    }

    case NPPN_BEFORESHUTDOWN:
    {
        m_bAboutToClose = true;
        break;
    }

    case NPPN_READY:
    {
        // This is workaround where dialog does not show tree on launch
        if (m_pJsonViewDlg && m_pJsonViewDlg->isVisible() && !m_bAboutToClose)
        {
            ::SendMessage(m_pJsonViewDlg->getHSelf(), WM_COMMAND, IDC_BTN_REFRESH, 0);
            m_pJsonViewDlg->UpdateTitle();
        }
        m_bNppReady = true;
        break;
    }

    default:
        return;
    }
}

LRESULT NppJsonPlugin::MessageProc(UINT /*msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return TRUE;
}

BOOL NppJsonPlugin::IsUnicode()
{
#ifdef _UNICODE
    return TRUE;
#else
    return FALSE;
#endif    //  _UNICODE
}

void NppJsonPlugin::SetMenuIcon()
{
    if (m_hMenuIcon.hToolbarIcon || m_hMenuIcon.hToolbarBmp)
    {
        toolbarIconsWithDarkMode tbIcon {};
        tbIcon.hToolbarBmp  = m_hMenuIcon.hToolbarBmp;
        tbIcon.hToolbarIcon = m_hMenuIcon.hToolbarIcon;
        tbIcon.hToolbarIconDarkMode = m_hMenuIcon.hToolbarIcon;
        auto nCommandId     = m_shortcutCommands.GetCommandID(CallBackID::SHOW_DOC_PANEL);
        ::SendMessage(m_NppData._nppHandle, NPPM_ADDTOOLBARICON_FORDARKMODE, reinterpret_cast<WPARAM&>(nCommandId), reinterpret_cast<LPARAM>(&tbIcon));
    }
}

void NppJsonPlugin::InitCommandMenu()
{
    m_shortcutCommands.SetShortCut(CallBackID::SHOW_DOC_PANEL, {true, true, true, 'J'});
    m_shortcutCommands.SetCommand(CallBackID::SHOW_DOC_PANEL, MENU_SHOW_JSON_PANEL, Callback::ShowJsonDlg, false);

    m_shortcutCommands.SetShortCut(CallBackID::FORMAT, {true, true, true, 'M'});
    m_shortcutCommands.SetCommand(CallBackID::FORMAT, MENU_FORMAT_JSON, Callback::FormatJson, false);

    m_shortcutCommands.SetShortCut(CallBackID::COMPRESS, {true, true, true, 'C'});
    m_shortcutCommands.SetCommand(CallBackID::COMPRESS, MENU_COMPRESS_JSON, Callback::CompressJson, false);

    m_shortcutCommands.SetShortCut(CallBackID::SORT_BY_KEY, {true, true, true, 'K'});
    m_shortcutCommands.SetCommand(CallBackID::SORT_BY_KEY, MENU_SORT_BY_KEY, Callback::SortJsonByKey, false);

    m_shortcutCommands.SetCommand(CallBackID::SEP_1, MENU_SEPERATOR, NULL, true);

    m_shortcutCommands.SetCommand(CallBackID::SETTING, MENU_SETTING, Callback::OpenSettingDlg, false);
    m_shortcutCommands.SetCommand(CallBackID::ABOUT, MENU_ABOUT, Callback::ShowAboutDlg, false);
}

void NppJsonPlugin::InitToolbarIcon()
{
    auto dpi                 = GetDeviceCaps(GetWindowDC(m_NppData._nppHandle), LOGPIXELSX);
    int  size                = 16 * dpi / 96;
    m_hMenuIcon.hToolbarIcon = reinterpret_cast<HICON>(::LoadImage(static_cast<HINSTANCE>(m_hModule), MAKEINTRESOURCE(IDI_ICON_TOOLBAR), IMAGE_ICON, size, size, 0));
    ICONINFO iconInfo;
    GetIconInfo(m_hMenuIcon.hToolbarIcon, &iconInfo);
    m_hMenuIcon.hToolbarBmp = iconInfo.hbmColor;
}

void NppJsonPlugin::InitConfigPath()
{
    // Get config dir path
    WCHAR szPath[_MAX_PATH] {};
    SendMessage(m_NppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(&szPath));
    m_configPath = std::wstring(szPath) + TEXT("\\") + PLUGIN_CONFIG;
}

void NppJsonPlugin::ToggleMenuItemState(int nCmdId, bool bVisible)
{
    ::SendMessage(m_NppData._nppHandle, NPPM_SETMENUITEMCHECK, static_cast<WPARAM>(nCmdId), bVisible);
}

void NppJsonPlugin::ConstructJsonDlg()
{
    if (!m_pJsonViewDlg)
    {
        ConstructSetting();
        auto nCmdId    = m_shortcutCommands.GetCommandID(CallBackID::SHOW_DOC_PANEL);
        m_pJsonViewDlg = std::make_unique<JsonViewDlg>(reinterpret_cast<HINSTANCE>(m_hModule), m_NppData, m_bNppReady, nCmdId, m_pSetting);
    }
}

void NppJsonPlugin::ConstructSetting()
{
    if (!m_pSetting)
    {
        m_pSetting = std::make_shared<Setting>();
        ProfileSetting(m_configPath).GetSettings(*m_pSetting);
    }
}

void NppJsonPlugin::ShowJsonDlg()
{
    ConstructJsonDlg();

    if (m_pJsonViewDlg)    // Hope it is constructed by now.
    {
        bool bVisible = !m_pJsonViewDlg->isVisible();
        m_pJsonViewDlg->ShowDlg(bVisible);
    }
}

void NppJsonPlugin::FormatJson()
{
    ConstructJsonDlg();

    if (m_pJsonViewDlg)    // Hope it is constructed by now.
    {
        m_pJsonViewDlg->FormatJson();
    }
}

void NppJsonPlugin::CompressJson()
{
    ConstructJsonDlg();

    if (m_pJsonViewDlg)    // Hope it is constructed by now.
    {
        m_pJsonViewDlg->CompressJson();
    }
}

void NppJsonPlugin::SortJsonByKey()
{
    ConstructJsonDlg();

    if (m_pJsonViewDlg)    // Hope it is constructed by now.
    {
        m_pJsonViewDlg->SortJsonByKey();
    }
}

void NppJsonPlugin::OpenSettingDlg()
{
    ConstructSetting();
    auto nCmdId = m_shortcutCommands.GetCommandID(CallBackID::SETTING);

    if (!m_pSettingsDlg)
        m_pSettingsDlg = std::make_unique<SettingsDlg>(reinterpret_cast<HINSTANCE>(m_hModule), m_NppData._nppHandle, nCmdId, m_configPath, m_pSetting);
    bool isShown = m_pSettingsDlg->ShowDlg(true);

    ToggleMenuItemState(nCmdId, isShown);
}

void NppJsonPlugin::ShowAboutDlg()
{
    auto nCmdId = m_shortcutCommands.GetCommandID(CallBackID::ABOUT);

    if (!m_pAboutDlg)
        m_pAboutDlg = std::make_unique<AboutDlg>(reinterpret_cast<HINSTANCE>(m_hModule), m_NppData._nppHandle, nCmdId);
    bool isShown = m_pAboutDlg->ShowDlg(true);

    ToggleMenuItemState(nCmdId, isShown);
}
