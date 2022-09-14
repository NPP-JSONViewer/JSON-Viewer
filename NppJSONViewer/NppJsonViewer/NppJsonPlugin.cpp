#include "NppJsonPlugin.h"
#include "resource.h"
#include <tchar.h>
#include "AboutDlg.h"

NppJsonPlugin* NppJsonPlugin::Callback::m_pNppJsonPlugin = nullptr;

NppJsonPlugin::NppJsonPlugin() : m_shortcutCommands(nTotalCommandCount)
{
	NppJsonPlugin::Callback::m_pNppJsonPlugin = this;
}

void NppJsonPlugin::PluginInit(HMODULE hModule)
{
	m_hModule = hModule;
}

void NppJsonPlugin::PluginCleanup()
{
}

void NppJsonPlugin::SetInfo(const NppData& notpadPlusData)
{
	m_NppData = notpadPlusData;
	InitCommandMenu();
	InitToolbarIcon();
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
		PluginCleanup();
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
#ifdef  _UNICODE
	return TRUE;
#else
	return FALSE;
#endif //  _UNICODE
}

void NppJsonPlugin::SetMenuIcon()
{
	if (m_hMenuIcon.hToolbarIcon || m_hMenuIcon.hToolbarBmp)
	{
		toolbarIcons tbIcon;
		tbIcon.hToolbarBmp = m_hMenuIcon.hToolbarBmp;
		tbIcon.hToolbarIcon = m_hMenuIcon.hToolbarIcon;
		auto nCommandId = m_shortcutCommands.GetCommandID(CallBackID::SHOW_DOC_PANEL);
		::SendMessage(m_NppData._nppHandle, NPPM_ADDTOOLBARICON, reinterpret_cast<WPARAM&>(nCommandId), reinterpret_cast<LPARAM>(&tbIcon));
	}
}

void NppJsonPlugin::InitCommandMenu()
{
	m_shortcutCommands.SetShortCut(CallBackID::SHOW_DOC_PANEL, { true, true, true, 'J' });
	m_shortcutCommands.SetCommand(CallBackID::SHOW_DOC_PANEL, MENU_SHOW_JSON_PANEL, Callback::ShowJsonDocPanelDlg, false);

	m_shortcutCommands.SetShortCut(CallBackID::FORMAT, { true, true, true, 'M' });
	m_shortcutCommands.SetCommand(CallBackID::FORMAT, MENU_FORMAT_JSON, Callback::FormatJson, false);

	m_shortcutCommands.SetShortCut(CallBackID::COMPRESS, { true, true, true, 'C' });
	m_shortcutCommands.SetCommand(CallBackID::COMPRESS, MENU_COMPRESS_JSON, Callback::CompressJson, false);

	m_shortcutCommands.SetCommand(CallBackID::SEP_1, MENU_SEPERATOR, NULL, true);

	m_shortcutCommands.SetCommand(CallBackID::OPTION, MENU_OPTION, Callback::OpenOptionDlg, false);
	m_shortcutCommands.SetCommand(CallBackID::ABOUT, MENU_ABOUT, Callback::ShowAboutDlg, false);
}

void NppJsonPlugin::InitToolbarIcon()
{
	auto dpi = GetDeviceCaps(GetWindowDC(m_NppData._nppHandle), LOGPIXELSX);
	int size = 16 * dpi / 96;
	m_hMenuIcon.hToolbarIcon = reinterpret_cast<HICON>(::LoadImage(static_cast<HINSTANCE>(m_hModule), MAKEINTRESOURCE(IDI_ICON_TOOLBAR), IMAGE_ICON, size, size, 0));
	ICONINFO iconinfo;
	GetIconInfo(m_hMenuIcon.hToolbarIcon, &iconinfo);
	m_hMenuIcon.hToolbarBmp = iconinfo.hbmColor;
}

void NppJsonPlugin::ToggleMenuItemState(int nCmdId, bool bVisible)
{
	::SendMessage(m_NppData._nppHandle, NPPM_SETMENUITEMCHECK, static_cast<WPARAM>(nCmdId), bVisible);
}

void NppJsonPlugin::ShowJsonDocPanelDlg()
{
	auto nCmdId = m_shortcutCommands.GetCommandID(CallBackID::SHOW_DOC_PANEL);

	if (!m_pTreeViewDocPanel)
	{
		m_pTreeViewDocPanel = std::make_unique<JsonTreeViewDlg>(reinterpret_cast<HINSTANCE>(m_hModule), m_NppData, nCmdId);
	}

	if (m_pTreeViewDocPanel)	// Hope it is constructed by now.
	{
		bool bVisible = !m_pTreeViewDocPanel->isVisible();
		m_pTreeViewDocPanel->ShowDlg(bVisible);
	}
}

void NppJsonPlugin::FormatJson()
{
	auto nCmdId = m_shortcutCommands.GetCommandID(CallBackID::FORMAT);

	if (!m_pAboutDlg)
		m_pAboutDlg = std::make_unique<AboutDlg>(reinterpret_cast<HINSTANCE>(m_hModule), m_NppData._nppHandle, nCmdId);
	bool isShown = m_pAboutDlg->ShowDlg(true);

	ToggleMenuItemState(nCmdId, isShown);
}

void NppJsonPlugin::CompressJson()
{
	auto nCmdId = m_shortcutCommands.GetCommandID(CallBackID::FORMAT);

	if (!m_pAboutDlg)
		m_pAboutDlg = std::make_unique<AboutDlg>(reinterpret_cast<HINSTANCE>(m_hModule), m_NppData._nppHandle, nCmdId);
	bool isShown = m_pAboutDlg->ShowDlg(true);

	ToggleMenuItemState(nCmdId, isShown);
}

void NppJsonPlugin::OpenOptionDlg()
{
	auto nCmdId = m_shortcutCommands.GetCommandID(CallBackID::OPTION);

	if (!m_pAboutDlg)
		m_pAboutDlg = std::make_unique<AboutDlg>(reinterpret_cast<HINSTANCE>(m_hModule), m_NppData._nppHandle, nCmdId);
	bool isShown = m_pAboutDlg->ShowDlg(true);

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
