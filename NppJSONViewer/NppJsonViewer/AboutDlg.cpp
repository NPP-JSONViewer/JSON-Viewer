#include "AboutDlg.h"
#include "resource.h"
#include "Utility.h"
#include "StringHelper.h"
#include "Define.h"
#include <string>
#include <commctrl.h>


AboutDlg::AboutDlg(HINSTANCE hInstance, HWND hParent, int nCmdId)
    : m_nCmdId(nCmdId)
    , StaticDialog()
{
    init(hInstance, hParent);
}


bool AboutDlg::ShowDlg(bool bShow)
{
    bool bShouldShow = bShow && !isVisible();
    if (bShouldShow)
    {
        if (!isCreated())
            create(IDD_ABOUTDLG);

        // Adjust the position of AboutBox
        goToCenter();
    }
    else
    {
        SendMessage(_hSelf, WM_COMMAND, IDCANCEL, NULL);
    }
    return bShouldShow;
}


INT_PTR AboutDlg::run_dlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    AboutDlg *pSelf = nullptr;
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        ::SetWindowLongPtr(_hSelf, DWLP_USER, lParam);

        pSelf = reinterpret_cast<AboutDlg *>(static_cast<LONG_PTR>(::GetWindowLongPtr(_hSelf, DWLP_USER)));
        if (pSelf)
        {
            pSelf->SetVersion(_hSelf);
        }
        SetFocus(GetDlgItem(_hSelf, IDOK));

        // Set links
        std::wstring urlIssue(TEXT("<a href=\"__URL__\">__URL__</a>"));
        std::wstring urlRepo = urlIssue;

        urlIssue = StringHelper::ReplaceAll(urlIssue, TEXT("__URL__"), URL_REPORT_ISSUE);
        urlRepo  = StringHelper::ReplaceAll(urlRepo, TEXT("__URL__"), URL_SOURCE_CODE);

        SetWindowText(::GetDlgItem(_hSelf, IDC_WEB_ISSUE), urlIssue.c_str());
        SetWindowText(::GetDlgItem(_hSelf, IDC_WEB_SOURCE), urlRepo.c_str());

        return TRUE;
    }

    case WM_NOTIFY:
    {
        switch (reinterpret_cast<LPNMHDR>(lParam)->code)
        {
        case NM_CLICK:
        case NM_RETURN:
        {
            auto  nmLink = reinterpret_cast<PNMLINK>(lParam);
            LITEM item   = nmLink->item;

            ShellExecute(nullptr, L"open", item.szUrl, nullptr, nullptr, SW_SHOW);
            return TRUE;
        }
        }
        return FALSE;
    }

    case WM_COMMAND:
    {
        pSelf = reinterpret_cast<AboutDlg *>(static_cast<LONG_PTR>(::GetWindowLongPtr(_hSelf, DWLP_USER)));
        switch (LOWORD(wParam))
        {
        case IDCANCEL:    // Close this dialog when clicking to close button
        case IDOK:
            if (pSelf)
                ::SendMessage(pSelf->_hParent, NPPM_SETMENUITEMCHECK, static_cast<WPARAM>(pSelf->m_nCmdId), false);
            EndDialog(_hSelf, wParam);
            _hSelf = nullptr;
            return TRUE;
        }
    }
    }
    return FALSE;
}

void AboutDlg::SetVersion(HWND hWnd)
{
    std::wstring version;

    // Get module path
    wchar_t moduleFileName[MAX_PATH + 1] = {};
    ::GetModuleFileName(static_cast<HMODULE>(getHinst()), moduleFileName, _MAX_PATH);

    version = CUtility::GetVersion(moduleFileName);
    if (!version.empty())
    {
        std::wstring text(PLUGIN_NAME);
        text += TEXT(" (");
        text += STR_VERSION;
        text += version;
        text += TEXT(") ");
        ::SetWindowText(::GetDlgItem(hWnd, IDC_GB_TITLE), text.c_str());
    }
}
