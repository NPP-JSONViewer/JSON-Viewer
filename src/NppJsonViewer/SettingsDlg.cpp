#include "SettingsDlg.h"
#include "resource.h"
#include "Utility.h"
#include "Profile.h"
#include <commctrl.h>
#include <Uxtheme.h>


SettingsDlg::SettingsDlg(HINSTANCE hInstance, HWND hParent, int nCmdId, const std::wstring& configPath, std::shared_ptr<Setting>& pSetting)
    : m_nCmdId(nCmdId)
    , m_configPath(configPath)
    , StaticDialog()
    , m_pSetting(pSetting)
{
    init(hInstance, hParent);
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

        EnableThemeDialogTexture(_hSelf, ETDT_ENABLETAB);

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
                ::MessageBox(_hSelf, JSON_ERR_SAVE_SETTING, JSON_ERROR_TITLE, MB_OK | MB_ICONERROR);
            }
            return TRUE;

        case IDCANCEL:    // Close this dialog when clicking to close button
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
        m_pSetting->lineEnding = LineEnding::AUTO;
    else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINE_WINDOW))
        m_pSetting->lineEnding = LineEnding::WINDOWS;
    else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINE_UNIX))
        m_pSetting->lineEnding = LineEnding::UNIX;
    else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINE_MAC))
        m_pSetting->lineEnding = LineEnding::MAC;

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
        m_pSetting->lineFormat = LineFormat::DEFAULT;
    else if (IsDlgButtonChecked(_hSelf, IDC_RADIO_LINEFORMAT_SINGLE))
        m_pSetting->lineFormat = LineFormat::SINGLELINE;

    m_pSetting->bFollowCurrentTab                 = CUtility::GetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_FOLLOW_CURRENT_DOC));
    m_pSetting->bAutoFormat                       = CUtility::GetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_FORMAT_ON_OPEN));
    m_pSetting->bUseJsonHighlight                 = CUtility::GetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_JSON_HIGHLIGHT));
    m_pSetting->parseOptions.bIgnoreTrailingComma = CUtility::GetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_IGNORE_COMMA));
    m_pSetting->parseOptions.bIgnoreComment       = CUtility::GetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_IGNORE_COMMENT));
    m_pSetting->parseOptions.bReplaceUndefined    = CUtility::GetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_REPLACE_UNDEFINED));

    return WriteINI();
}

void SettingsDlg::destroy() {}

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
    switch (m_pSetting->lineEnding)
    {
    case LineEnding::WINDOWS:
        nCtrlID = IDC_RADIO_LINE_WINDOW;
        break;
    case LineEnding::UNIX:
        nCtrlID = IDC_RADIO_LINE_UNIX;
        break;
    case LineEnding::MAC:
        nCtrlID = IDC_RADIO_LINE_MAC;
        break;
    case LineEnding::AUTO:
        nCtrlID = IDC_RADIO_LINE_AUTO;
        break;
    default:
        break;
    }
    CheckRadioButton(_hSelf, IDC_RADIO_LINE_AUTO, IDC_RADIO_LINE_MAC, nCtrlID);


    nCtrlID = IDC_RADIO_LINEFORMAT_DEFAULT;
    switch (m_pSetting->lineFormat)
    {
    case LineFormat::SINGLELINE:
        nCtrlID = IDC_RADIO_LINEFORMAT_SINGLE;
        break;
    case LineFormat::DEFAULT:
        nCtrlID = IDC_RADIO_LINEFORMAT_DEFAULT;
        break;
    default:
        break;
    }
    CheckRadioButton(_hSelf, IDC_RADIO_LINEFORMAT_DEFAULT, IDC_RADIO_LINEFORMAT_SINGLE, nCtrlID);


    switch (m_pSetting->indent.style)
    {
    case IndentStyle::AUTO:
        nCtrlID = IDC_RADIO_INDENT_AUTO;
        break;
    case IndentStyle::TAB:
        nCtrlID = IDC_RADIO_INDENT_TAB;
        break;
    case IndentStyle::SPACE:
        nCtrlID = IDC_RADIO_INDENT_SPACE;
        break;
    default:
        break;
    }
    CheckRadioButton(_hSelf, IDC_RADIO_INDENT_AUTO, IDC_RADIO_INDENT_SPACE, nCtrlID);
    CUtility::SetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_INDENT_SPACECOUNT), std::to_wstring(m_pSetting->indent.len));
    ShowSpaceCountCtrls(m_pSetting->indent.style == IndentStyle::SPACE);

    CUtility::SetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_FOLLOW_CURRENT_DOC), m_pSetting->bFollowCurrentTab);
    CUtility::SetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_FORMAT_ON_OPEN), m_pSetting->bAutoFormat);
    CUtility::SetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_JSON_HIGHLIGHT), m_pSetting->bUseJsonHighlight);
    CUtility::SetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_IGNORE_COMMA), m_pSetting->parseOptions.bIgnoreTrailingComma);
    CUtility::SetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_IGNORE_COMMENT), m_pSetting->parseOptions.bIgnoreComment);
    CUtility::SetCheckboxStatus(::GetDlgItem(_hSelf, IDC_CHK_REPLACE_UNDEFINED), m_pSetting->parseOptions.bReplaceUndefined);
}

void SettingsDlg::ShowSpaceCountCtrls(bool bShow)
{
    auto show = bShow ? SW_SHOW : SW_HIDE;
    ShowWindow(::GetDlgItem(_hSelf, IDC_EDT_INDENT_SPACECOUNT), show);
    ShowWindow(::GetDlgItem(_hSelf, IDC_STATIC_SPACECOUNT), show);
}
