#include "SettingsDlg.h"
#include "resource.h"
#include "Utility.h"
#include "Profile.h"
#include <commctrl.h>
#include <Uxtheme.h>


SettingsDlg::SettingsDlg(HINSTANCE hIntance, HWND hParent, int nCmdId, const std::wstring& configPath)
	: m_nCmdId(nCmdId)
	, m_configPath(configPath)
	, StaticDialog()
	, m_pSetting(std::make_unique<Setting>())
{
	init(hIntance, hParent);
}


bool SettingsDlg::ShowDlg(bool bShow)
{
	bool bShouldShow = bShow && !isVisible();
	if (bShouldShow)
	{
		if (!isCreated())
			create(IDD_SETTING);

		// Adjust the position of Setting dialog
		goToCenter();
	}
	else
	{
		SendMessage(_hSelf, WM_COMMAND, IDCANCEL, NULL);
	}
	return bShouldShow;
}

INT_PTR SettingsDlg::run_dlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		::SetWindowLongPtr(_hSelf, DWLP_USER, lParam);

		auto enable_dlg_theme = reinterpret_cast<ETDTProc>(::SendMessage(_hParent, NPPM_GETENABLETHEMETEXTUREFUNC, 0, 0));
		if (enable_dlg_theme != nullptr)
			enable_dlg_theme(_hSelf, ETDT_ENABLETAB);

		InitDlg();

		SetFocus(GetDlgItem(_hSelf, IDOK));

		return TRUE;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (Apply())
			{
				::SendMessage(_hParent, NPPM_SETMENUITEMCHECK, static_cast<WPARAM>(m_nCmdId), false);
				EndDialog(_hSelf, wParam);
			}
			else
			{
				::MessageBox(_hSelf, L"Failed to save the setting. Please try again.",
					L"Setting Save Error", MB_OK | MB_ICONERROR);
			}
			return TRUE;

		case IDCANCEL: // Close this dialog when clicking to close button
			::SendMessage(_hParent, NPPM_SETMENUITEMCHECK, static_cast<WPARAM>(m_nCmdId), false);
			EndDialog(_hSelf, wParam);
			return TRUE;

		case IDC_RADIO_INDENT_AUTO:
		case IDC_RADIO_INDENT_TAB:
			ShowSpaceCountCtrls(false);
			return TRUE;

		case IDC_RADIO_INDENT_SPACE:
			ShowSpaceCountCtrls(true);
			return TRUE;

		}
	}
	}
	return FALSE;
}

bool SettingsDlg::Apply()
{
	// Get all the data from the UI

	// Line Ending setting
	if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINE_AUTO))
		m_pSetting->le = LineEnding::AUTO;
	else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINE_WINDOW))
		m_pSetting->le = LineEnding::WINDOWS;
	else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINE_UNIX))
		m_pSetting->le = LineEnding::UNIX;
	else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINE_MAC))
		m_pSetting->le = LineEnding::MAC;

	// Indentation setting
	if (IsDlgButtonChecked(_hSelf, IDC_RADIO_INDENT_AUTO))
		m_pSetting->indent.style = IndentStyle::AUTO;
	else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_INDENT_TAB))
		m_pSetting->indent.style = IndentStyle::TAB;
	else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_INDENT_SPACE))
		m_pSetting->indent.style = IndentStyle::SPACE;

	auto indent_len = CUtility::GetNumber(CUtility::GetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_INDENT_SPACECOUNT)));
	if (indent_len.has_value())
	{
		m_pSetting->indent.len = indent_len.value();
	}

	// Line Ending setting
	if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINEFORMAT_DEFAULT))
		m_pSetting->lf = LineFormat::DEFAULT;
	else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINEFORMAT_SINGLE))
		m_pSetting->lf = LineFormat::SINGLELINE;

	return WriteINI();
}

void SettingsDlg::destroy()
{
}

bool SettingsDlg::ReadINI()
{
	return ProfileSetting(m_configPath).GetSettings(*m_pSetting);
}

bool SettingsDlg::WriteINI()
{
	return ProfileSetting(m_configPath).SetSettings(*m_pSetting);
}

void SettingsDlg::InitDlg()
{
	ReadINI();

	// Set UI control state here

	int nCtrlID = IDC_RADIO_LINE_AUTO;
	switch (m_pSetting->le)
	{
	case LineEnding::WINDOWS:		nCtrlID = IDC_RADIO_LINE_WINDOW;		break;
	case LineEnding::UNIX:			nCtrlID = IDC_RADIO_LINE_UNIX;			break;
	case LineEnding::MAC:			nCtrlID = IDC_RADIO_LINE_MAC;			break;
	case LineEnding::AUTO:			nCtrlID = IDC_RADIO_LINE_AUTO;			break;
	default:																break;
	}
	CheckRadioButton(_hSelf, IDC_RADIO_LINE_AUTO, IDC_RADIO_LINE_MAC, nCtrlID);


	nCtrlID = IDC_RADIO_LINEFORMAT_DEFAULT;
	switch (m_pSetting->lf)
	{
	case LineFormat::SINGLELINE:	nCtrlID = IDC_RADIO_LINEFORMAT_SINGLE;	break;
	case LineFormat::DEFAULT:		nCtrlID = IDC_RADIO_LINEFORMAT_DEFAULT;	break;
	default:																break;
	}
	CheckRadioButton(_hSelf, IDC_RADIO_LINEFORMAT_DEFAULT, IDC_RADIO_LINEFORMAT_SINGLE, nCtrlID);


	switch (m_pSetting->indent.style)
	{
	case IndentStyle::AUTO:			nCtrlID = IDC_RADIO_INDENT_AUTO;		break;
	case IndentStyle::TAB:			nCtrlID = IDC_RADIO_INDENT_TAB;			break;
	case IndentStyle::SPACE:		nCtrlID = IDC_RADIO_INDENT_SPACE;		break;
	default:																break;
	}
	CheckRadioButton(_hSelf, IDC_RADIO_INDENT_AUTO, IDC_RADIO_INDENT_SPACE, nCtrlID);
	CUtility::SetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_INDENT_SPACECOUNT), std::to_wstring(m_pSetting->indent.len));
	ShowSpaceCountCtrls(m_pSetting->indent.style == IndentStyle::SPACE);

}

void SettingsDlg::ShowSpaceCountCtrls(bool bShow)
{
	auto show = bShow ? SW_SHOW : SW_HIDE;
	ShowWindow(::GetDlgItem(_hSelf, IDC_EDT_INDENT_SPACECOUNT), show);
	ShowWindow(::GetDlgItem(_hSelf, IDC_STATIC_SPACECOUNT), show);
}
