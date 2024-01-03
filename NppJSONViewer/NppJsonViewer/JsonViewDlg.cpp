#include "JsonViewDlg.h"
#include "Define.h"
#include "Utility.h"
#include "StringHelper.h"
#include "RapidJsonHandler.h"
#include "ScintillaEditor.h"
#include "Profile.h"
#include <format>
#include <regex>


JsonViewDlg::JsonViewDlg(HINSTANCE hInstance, const NppData &nppData, const bool &isReady, int nCmdId, std::shared_ptr<Setting> &pSetting)
    : DockingDlgInterface(IDD_TREEDLG)
    , m_NppData(nppData)
    , m_IsNppReady(isReady)
    , m_nDlgId(nCmdId)
    , m_Editor(std::make_unique<ScintillaEditor>(nppData))
    , m_hTreeView(std::make_unique<TreeViewCtrl>())
    , m_pSetting(pSetting)
{
    _hParent = nppData._nppHandle;
    _hInst   = hInstance;
}

JsonViewDlg::~JsonViewDlg()
{
    for (HICON hIcon : m_hBtnIcon)
    {
        if (hIcon != nullptr)
            DestroyIcon(hIcon);
    }
}

void JsonViewDlg::ShowDlg(bool bShow)
{
    if (!isCreated())
    {
        init(_hInst, _hParent);
        tTbData data {};
        create(&data);

        // Set Initial rect, width and Height
        getWindowRect(m_rcInitialWindowRect);

        RECT rc {};
        getClientRect(rc);
        m_lfInitialClientWidth  = rc.right - rc.left;
        m_lfInitialClientHeight = rc.bottom - rc.top;

        // define the default docking behaviour
        data.uMask         = DWS_DF_CONT_LEFT | DWS_ICONTAB;
        data.pszModuleName = getPluginFileName();
        data.pszName       = const_cast<TCHAR *>(TITLE_JSON_PANEL);
        data.hIconTab      = static_cast<HICON>(LoadImage(_hInst, MAKEINTRESOURCE(IDI_ICON_TOOLBAR), IMAGE_ICON, 32, 32, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT));

        // the dlgDlg should be the index of funcItem where the current function pointer is
        data.dlgID = static_cast<int>(CallBackID::SHOW_DOC_PANEL);
        ::SendMessage(_hParent, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
    }

    if (bShow)
    {
        // Draw json tree now
        DrawJsonTree();
    }

    DockingDlgInterface::display(bShow);
}

void JsonViewDlg::FormatJson()
{
    const auto selectedText              = m_Editor->GetJsonText();
    auto [le, lf, indentChar, indentLen] = GetFormatSetting();

    Result res = JsonHandler(m_pSetting->parseOptions).FormatJson(selectedText, le, lf, indentChar, indentLen);

    if (res.success)
    {
        m_Editor->ReplaceSelection(res.response);
        HighlightAsJson();
    }
    else
    {
        if (CheckForTokenUndefined(JsonViewDlg::eMethod::FormatJson, selectedText, res, NULL))
            return;

        ReportError(res);
    }
}

void JsonViewDlg::CompressJson()
{
    // Get the current scintilla
    const auto selectedText = m_Editor->GetJsonText();

    Result res = JsonHandler(m_pSetting->parseOptions).GetCompressedJson(selectedText);

    if (res.success)
    {
        m_Editor->ReplaceSelection(res.response);
        HighlightAsJson();
    }
    else
    {
        if (CheckForTokenUndefined(JsonViewDlg::eMethod::GetCompressedJson, selectedText, res, NULL))
            return;

        ReportError(res);
    }
}

bool JsonViewDlg::CheckForTokenUndefined(eMethod method, std::string selectedText, Result &res, HTREEITEM tree_root)
{
    auto [le, lf, indentChar, indentLen] = GetFormatSetting();

    if (m_pSetting->parseOptions.bReplaceUndefined)
    {
        auto text = selectedText.substr(res.error_pos, 9);
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char c)
            {
                return (unsigned char)std::tolower(c);
            });
        if (text == "undefined")
        {
            try
            {
                std::regex regex("([:\\[,])([\\s]*?)undefined([\\s,}]*?)", std::regex_constants::icase);
                text = std::regex_replace(selectedText, regex, "$1$2null");
                switch (method)
                {
                case eMethod::FormatJson:
                    res = JsonHandler(m_pSetting->parseOptions).FormatJson(text, le, lf, indentChar, indentLen);
                    break;
                case eMethod::GetCompressedJson:
                    res = JsonHandler(m_pSetting->parseOptions).GetCompressedJson(text);
                    break;
                case eMethod::ParseJson:
                {
                    RapidJsonHandler        handler(this, tree_root);
                    rapidjson::StringBuffer sb;
                    res = JsonHandler(m_pSetting->parseOptions).ParseJson<flgBaseReader>(text, sb, handler);
                    break;
                }
                case eMethod::ValidateJson:
                    res = JsonHandler(m_pSetting->parseOptions).ValidateJson(text);
                    break;
                }
                if (res.success)
                {
                    m_Editor->ReplaceSelection((method == eMethod::ParseJson || method == eMethod::ValidateJson) ? text : res.response);
                    HighlightAsJson();
                    return true;
                }
                else
                {
                    m_Editor->ReplaceSelection(text);
                    m_Editor->MakeSelection(m_Editor->GetSelectionStart(), static_cast<int>(text.length()));
                    m_Editor->RefreshSelectionPos();
                }
            }
            catch (const std::exception &)
            {
            }
        }
    }
    return false;
}

void JsonViewDlg::HandleTabActivated()
{
    const bool bIsVisible = isCreated() && isVisible();
    if (bIsVisible)
    {
        m_Editor->RefreshViewHandle();
        if (m_Editor->IsJsonFile())
        {
            if (m_pSetting->bFollowCurrentTab)
            {
                DrawJsonTree();
            }

            if (m_pSetting->bAutoFormat)
            {
                FormatJson();
            }
        }
    }
}

void JsonViewDlg::ValidateJson()
{
    // Get the current scintilla
    const auto selectedText = m_Editor->GetJsonText();

    Result res = JsonHandler(m_pSetting->parseOptions).ValidateJson(selectedText);

    if (res.success)
    {
        ShowMessage(JSON_INFO_TITLE, JSON_ERR_VALIDATE_SUCCESS, MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        if (CheckForTokenUndefined(JsonViewDlg::eMethod::ValidateJson, selectedText, res, NULL))
        {
            ShowMessage(JSON_INFO_TITLE, JSON_ERR_VALIDATE_SUCCESS, MB_OK | MB_ICONINFORMATION);
            return;
        }

        ReportError(res);
    }
}

void JsonViewDlg::DrawJsonTree()
{
    // Disable all buttons and treeView
    std::vector<DWORD> ctrls = {IDC_BTN_REFRESH, IDC_BTN_VALIDATE, IDC_BTN_FORMAT, IDC_BTN_SEARCH, IDC_EDT_SEARCH};
    EnableControls(ctrls, false);

    HTREEITEM rootNode = nullptr;
    rootNode           = m_hTreeView->InitTree();

    // Refresh the view
    m_Editor->RefreshViewHandle();
    const std::string txtForParsing = m_Editor->GetJsonText();

    if (txtForParsing.empty())
    {
        m_hTreeView->InsertNode(JSON_ERR_PARSE, NULL, rootNode);
    }
    else
    {
        auto res = PopulateTreeUsingSax(rootNode, txtForParsing);
        if (res.has_value())
        {
            // This is the case when Notepad++ has JsonViewer Window opened for previous instance
            // Later on second launch, don't show the error message as this could be some text file
            // If it is real json file but has some error, then there must be more than 1 node exist.

            if (!m_IsNppReady && m_hTreeView->GetNodeCount() <= 1)
            {
                m_hTreeView->InsertNode(JSON_ERR_VALIDATE, NULL, rootNode);
            }
            else
            {
                ShowMessage(JSON_ERROR_TITLE, res.value(), MB_OK | MB_ICONERROR);
            }
        }
    }

    m_hTreeView->Expand(rootNode);

    // Enable all buttons and treeView
    EnableControls(ctrls, true);
}

void JsonViewDlg::HighlightAsJson(bool bForcefully) const
{
    bool setJsonLang = bForcefully || m_pSetting->bUseJsonHighlight;
    if (setJsonLang)
        m_Editor->SetLangAsJson();
}

auto JsonViewDlg::PopulateTreeUsingSax(HTREEITEM tree_root, const std::string &jsonText) -> std::optional<std::wstring>
{
    std::optional<std::wstring> retVal = std::nullopt;

    RapidJsonHandler        handler(this, tree_root);
    rapidjson::StringBuffer sb;

    Result res = JsonHandler(m_pSetting->parseOptions).ParseJson<flgBaseReader>(jsonText, sb, handler);
    if (!res.success)
    {
        if (CheckForTokenUndefined(JsonViewDlg::eMethod::ParseJson, jsonText, res, tree_root))
            return retVal;

        // Intimate user
        if (jsonText.empty())
        {
            retVal = std::make_optional<std::wstring>(JSON_ERR_PARSE);
        }
        else
        {
            // Mark the error position
            size_t start       = m_Editor->GetSelectionStart();
            size_t errPosition = start + static_cast<size_t>(res.error_pos);
            m_Editor->MakeSelection(errPosition, errPosition + 1);

            std::string err = std::format("\n\nError: ({} : {})", res.error_code, res.error_str);
            retVal          = std::make_optional<std::wstring>((JSON_ERR_VALIDATE + StringHelper::ToWstring(err)));
        }
    }
    else
    {
        HighlightAsJson();
    }

    return retVal;
}

HTREEITEM JsonViewDlg::InsertToTree(HTREEITEM parent, const std::string &text)
{
    auto wText = StringHelper::ToWstring(text, CP_UTF8);
    return m_hTreeView->InsertNode(wText, NULL, parent);
}

void JsonViewDlg::AppendNodeCount(HTREEITEM node, unsigned elementCount, bool bArray)
{
    if (!node)
        return;

    auto txt = m_hTreeView->GetNodeName(node, false);

    txt += L" ";
    txt += bArray ? L"[" : L"{";
    txt += std::to_wstring(elementCount);
    txt += bArray ? L"]" : L"}";

    m_hTreeView->UpdateNodeText(node, txt);
}

void JsonViewDlg::UpdateNodePath(HTREEITEM htiNode)
{
    std::wstring nodePath = m_hTreeView->GetNodePath(htiNode);
    CUtility::SetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_NODEPATH), nodePath);
}

void JsonViewDlg::SearchInTree()
{
    std::wstring itemToSearch = CUtility::GetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_SEARCH));
    CUtility::SetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_NODEPATH), STR_SRCH_SEARCHING + itemToSearch);
    m_hTreeView->SetSelection(nullptr);

    static int          foundCount = 0;
    static std::wstring previousSearch;
    static HTREEITEM    nextNode = m_hTreeView->NextItem(m_hTreeView->GetRoot());

    // New search, hence search from beginning
    if (previousSearch != itemToSearch)
    {
        previousSearch = itemToSearch;
        nextNode       = m_hTreeView->NextItem(m_hTreeView->GetRoot());
        foundCount     = 0;
    }
    else
    {
        nextNode = m_hTreeView->NextItem(nextNode);
        if (nextNode == m_hTreeView->GetRoot())
        {
            nextNode   = m_hTreeView->NextItem(nextNode);
            foundCount = 0;
        }
    }

    // Check if this is an empty json
    std::wstring nodeText = m_hTreeView->GetNodeName(nextNode, true);
    if (nodeText.empty() || wcscmp(nodeText.c_str(), JSON_ERR_PARSE) == 0)
    {
        CUtility::SetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_NODEPATH), STR_SRCH_NOTFOUND + itemToSearch);
    }
    else
    {
        bool bFound = false;
        while (!bFound && nextNode)
        {
            nodeText     = m_hTreeView->GetNodeName(nextNode, true);
            auto nodeKey = m_hTreeView->GetNodeKey(nextNode);
            auto nodeVal = m_hTreeView->GetNodeValue(nextNode);

            // Search in node value
            //  1. If both key and value are not equal
            //  2. If both are equal, but not all three (key, value and keyValue)
            //  3. If all three equal, but key does not start with '[' and end with ']'

            bool shouldSearch = (nodeKey != nodeVal);
            shouldSearch |= (nodeKey == nodeVal && nodeKey != nodeText);
            shouldSearch |= (nodeKey == nodeVal && nodeKey == nodeText && !nodeKey.starts_with(L"[") && !nodeKey.ends_with(L"]"));
            if (shouldSearch)
                bFound = StringHelper::Contains(nodeVal, itemToSearch);

            // Search in Key if not found in value
            //  1. If key does not start with '[' and end with ']'

            shouldSearch = (!nodeKey.starts_with(L"[") && !nodeKey.ends_with(L"]"));
            if (!bFound && shouldSearch)
                bFound = StringHelper::Contains(nodeKey, itemToSearch);

            if (bFound)
                break;

            nextNode = m_hTreeView->NextItem(nextNode);
        }

        if (bFound)
        {
            UpdateNodePath(nextNode);
            m_hTreeView->SetSelection(nextNode);
            ++foundCount;
        }
        else
        {
            if (foundCount)
                CUtility::SetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_NODEPATH), STR_SRCH_NOMOREFOUND + itemToSearch);
            else
                CUtility::SetEditCtrlText(::GetDlgItem(_hSelf, IDC_EDT_NODEPATH), STR_SRCH_NOTFOUND + itemToSearch);
        }
    }
}

void JsonViewDlg::PrepareButtons()
{
    // Refresh Button
    SetIconAndTooltip(eButton::eRefresh, TOOLTIP_REFRESH);

    // validate Button
    SetIconAndTooltip(eButton::eValidate, TOOLTIP_VALIDATE);

    // Format button
    SetIconAndTooltip(eButton::eFormat, TOOLTIP_FORMAT);

    // Search button
    SetIconAndTooltip(eButton::eSearch, TOOLTIP_SEARCH);
}

void JsonViewDlg::SetIconAndTooltip(eButton ctrlType, const std::wstring &toolTip)
{
    int nCtrlID   = 0;
    int iconResID = 0;
    switch (ctrlType)
    {
    case eButton::eRefresh:
        nCtrlID   = IDC_BTN_REFRESH;
        iconResID = IDI_ICON_REFRESH;
        break;

    case eButton::eValidate:
        nCtrlID   = IDC_BTN_VALIDATE;
        iconResID = IDI_ICON_VALIDATE;
        break;

    case eButton::eFormat:
        nCtrlID   = IDC_BTN_FORMAT;
        iconResID = IDI_ICON_FORMAT;
        break;

    case eButton::eSearch:
        nCtrlID   = IDC_BTN_SEARCH;
        iconResID = IDI_ICON_SEARCH;
        break;

    default:
        return;
    }

    HWND hWnd = ::GetDlgItem(_hSelf, nCtrlID);
    RECT rc   = {};
    GetClientRect(hWnd, &rc);

    int nCtrlType         = static_cast<int>(ctrlType);
    int icon_size         = min(rc.bottom - rc.top, rc.right - rc.left) * 4 / 5;
    m_hBtnIcon[nCtrlType] = static_cast<HICON>(LoadImage(_hInst, MAKEINTRESOURCE(iconResID), IMAGE_ICON, icon_size, icon_size, 0));
    SendMessage(hWnd, BM_SETIMAGE, static_cast<WPARAM>(IMAGE_ICON), reinterpret_cast<LPARAM>(m_hBtnIcon[nCtrlType]));

    CUtility::CreateToolTip(_hSelf, nCtrlID, toolTip, _hInst);
}

void JsonViewDlg::AdjustDocPanelSize(int nWidth, int nHeight)
{
    // Calculate desktop scale.
    float fDeskScale = CUtility::GetDesktopScale(_hSelf);

    auto newDeltaWidth = nWidth - m_lfInitialClientWidth - 2;    // -2 is used for margin
    auto addWidth      = static_cast<int>((newDeltaWidth - m_lfDeltaWidth) * fDeskScale);
    m_lfDeltaWidth     = newDeltaWidth;

    auto newDeltaHeight = nHeight - m_lfInitialClientHeight;
    auto addHeight      = static_cast<int>((newDeltaHeight - m_lfDeltaHeight) * fDeskScale);
    m_lfDeltaHeight     = newDeltaHeight;

    // elements that need to be resized horizontally
    const auto resizeWindowIDs = {IDC_EDT_SEARCH, IDC_TREE};

    // elements that need to be moved
    const auto moveWindowIDs = {IDC_BTN_SEARCH};

    // elements which requires both resizing and move
    const auto resizeAndmoveWindowIDs = {IDC_EDT_NODEPATH};

    const UINT flags = SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_SHOWWINDOW;

    RECT rc;
    for (int id : resizeWindowIDs)
    {
        HWND hWnd = ::GetDlgItem(_hSelf, id);
        ::GetWindowRect(hWnd, &rc);
        int cx = rc.right - rc.left + addWidth;
        int cy = rc.bottom - rc.top;

        if (id == IDC_TREE)
            cy += addHeight;

        ::SetWindowPos(hWnd, NULL, 0, 0, cx, cy, SWP_NOMOVE | flags);
    }

    for (int id : moveWindowIDs)
    {
        HWND hWnd = GetDlgItem(_hSelf, id);
        ::GetWindowRect(hWnd, &rc);
        ::MapWindowPoints(NULL, _hSelf, (LPPOINT)&rc, 2);

        ::SetWindowPos(hWnd, NULL, rc.left + addWidth, rc.top, 0, 0, SWP_NOSIZE | flags);
    }

    for (int id : resizeAndmoveWindowIDs)
    {
        HWND hWnd = GetDlgItem(_hSelf, id);

        ::GetWindowRect(hWnd, &rc);
        int cx = rc.right - rc.left + addWidth;
        int cy = rc.bottom - rc.top;
        ::MapWindowPoints(NULL, _hSelf, (LPPOINT)&rc, 2);

        ::SetWindowPos(hWnd, NULL, rc.left, rc.top + addHeight, cx, cy, flags);
    }
}

void JsonViewDlg::ShowContextMenu(int x, int y)
{
    POINT p {.x = x, .y = y};

    TVHITTESTINFO tvHitInfo {.pt = p, .flags = 0, .hItem = nullptr};

    m_hTreeView->ScreenToTreeView(&(tvHitInfo.pt));

    // Detect if the given position is on the element TVITEM
    HTREEITEM hTreeItem = m_hTreeView->HitTest(&tvHitInfo);

    if (hTreeItem != nullptr)
    {
        // Make item selected
        m_hTreeView->SelectItem(hTreeItem);

        if (tvHitInfo.flags & (TVHT_ONITEM | TVHT_ONITEMBUTTON))
        {
            // Right click
            UpdateNodePath(hTreeItem);
            ShowContextMenu(hTreeItem, &p);
        }
    }
}

void JsonViewDlg::ShowContextMenu(HTREEITEM htiNode, LPPOINT lppScreen)
{
    // Select it
    m_hTreeView->SelectItem(htiNode);

    // Show menu
    if (lppScreen != NULL)
    {
        bool bEnableCopyName  = true;
        bool bEnableCopyValue = true;
        bool bEnableCopyPath  = true;

        bool bEnableExpand   = false;
        bool bEnableCollapse = false;

        if (m_hTreeView->GetRoot() == htiNode)
        {
            bEnableCopyName  = false;
            bEnableCopyValue = false;
            bEnableCopyPath  = false;
        }

        if (m_hTreeView->HasChild(htiNode))
        {
            bEnableCopyName  = false;
            bEnableCopyValue = false;
            bEnableCollapse  = m_hTreeView->IsThisOrAnyChildExpanded(htiNode);
            bEnableExpand    = m_hTreeView->IsThisOrAnyChildCollapsed(htiNode);
        }

        // Create menu
        HMENU hMenuPopup = CreatePopupMenu();
        UINT  itemFlag;

        itemFlag = MF_STRING | MF_ENABLED;
        AppendMenu(hMenuPopup, itemFlag, IDM_COPY_TREEITEM, STR_COPY);

        // separator
        AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);

        itemFlag = MF_STRING | (bEnableCopyName ? MF_ENABLED : MF_DISABLED);
        AppendMenu(hMenuPopup, itemFlag, IDM_COPY_NODENAME, STR_COPYNAME);

        itemFlag = MF_STRING | (bEnableCopyValue ? MF_ENABLED : MF_DISABLED);
        AppendMenu(hMenuPopup, itemFlag, IDM_COPY_NODEVALUE, STR_COPYVALUE);

        itemFlag = MF_STRING | (bEnableCopyPath ? MF_ENABLED : MF_DISABLED);
        AppendMenu(hMenuPopup, itemFlag, IDM_COPY_NODEPATH, STR_COPYPATH);

        // separator
        AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);

        itemFlag = MF_STRING | (bEnableExpand ? MF_ENABLED : MF_DISABLED);
        AppendMenu(hMenuPopup, itemFlag, IDM_EXPANDALL, STR_EXPANDALL);

        itemFlag = MF_STRING | (bEnableCollapse ? MF_ENABLED : MF_DISABLED);
        AppendMenu(hMenuPopup, itemFlag, IDM_COLLAPSEALL, STR_COLLAPSEALL);

        // Open menu
        TrackPopupMenu(hMenuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, lppScreen->x, lppScreen->y, 0, _hSelf, NULL);

        // Clean up
        DestroyMenu(hMenuPopup);
    }
}

void JsonViewDlg::ContextMenuExpand(bool bExpand)
{
    HTREEITEM htiSelected = m_hTreeView->GetSelection();
    if (htiSelected == NULL)
        return;

    HTREEITEM htiRoot = m_hTreeView->GetRoot();
    HTREEITEM htiNext = htiSelected;
    while (htiNext != NULL)
    {
        if (!(htiNext == htiRoot && !bExpand))
            bExpand ? m_hTreeView->Expand(htiNext) : m_hTreeView->Collapse(htiNext);
        htiNext = m_hTreeView->NextItem(htiNext, htiSelected);
    }
}

auto JsonViewDlg::CopyName() const -> std::wstring
{
    HTREEITEM selectedNode = m_hTreeView->GetSelection();
    if (selectedNode)
    {
        return m_hTreeView->GetNodeName(selectedNode, true);
    }
    return std::wstring();
}

auto JsonViewDlg::CopyKey() const -> std::wstring
{
    HTREEITEM selectedNode = m_hTreeView->GetSelection();
    if (selectedNode)
    {
        return m_hTreeView->GetNodeKey(selectedNode);
    }
    return std::wstring();
}

auto JsonViewDlg::CopyValue() const -> std::wstring
{
    HTREEITEM selectedNode = m_hTreeView->GetSelection();
    if (selectedNode)
    {
        return m_hTreeView->GetNodeValue(selectedNode);
    }
    return std::wstring();
}

auto JsonViewDlg::CopyPath() const -> std::wstring
{
    HTREEITEM selectedNode = m_hTreeView->GetSelection();
    if (selectedNode)
    {
        return m_hTreeView->GetNodePath(selectedNode);
    }
    return std::wstring();
}

int JsonViewDlg::ShowMessage(const std::wstring &title, const std::wstring &msg, int flag, bool bDontShow)
{
    return !bDontShow ? ::MessageBox(_hParent, msg.c_str(), title.c_str(), flag) : IDOK;
}

void JsonViewDlg::ReportError(const Result &result)
{
    // Mark the error position
    size_t start = m_Editor->GetSelectionStart() + result.error_pos;
    size_t end   = m_Editor->GetSelectionEnd();
    m_Editor->MakeSelection(start, end);

    // Intimate user
    std::string err = std::format("\n\nError: ({} : {})", result.error_code, result.error_str);

    ShowMessage(JSON_ERROR_TITLE, (JSON_ERR_VALIDATE + StringHelper::ToWstring(err)).c_str(), MB_OK | MB_ICONERROR);
}

void JsonViewDlg::ToggleMenuItemState(bool bVisible)
{
    ::SendMessage(_hParent, NPPM_SETMENUITEMCHECK, static_cast<WPARAM>(m_nDlgId), bVisible);
}

void JsonViewDlg::ShowControls(const std::vector<DWORD> &ids, bool show)
{
    for (auto id : ids)
        ShowWindow(GetDlgItem(getHSelf(), id), show ? SW_SHOW : SW_HIDE);
}

void JsonViewDlg::EnableControls(const std::vector<DWORD> &ids, bool enable)
{
    for (auto id : ids)
        EnableWindow(GetDlgItem(getHSelf(), id), enable ? TRUE : FALSE);
}

void JsonViewDlg::HandleTreeEvents(LPARAM lParam)
{
    LPNMHDR lpnmh = reinterpret_cast<LPNMHDR>(lParam);
    if (!lpnmh || lpnmh->idFrom != IDC_TREE)
        return;    // Not click inside JsonTree

    switch (lpnmh->code)
    {
    case TVN_SELCHANGED:
    {
        NMTREEVIEW *pnmtv = reinterpret_cast<LPNMTREEVIEW>(lParam);
        HTREEITEM   hItem = pnmtv->itemNew.hItem;
        if (hItem && (pnmtv->action == TVC_BYMOUSE || pnmtv->action == TVC_BYKEYBOARD))
        {
            UpdateNodePath(hItem);
        }
    }
    break;
    }
}

auto JsonViewDlg::GetFormatSetting() const -> std::tuple<LE, LF, char, unsigned>
{
    LE       le         = LE::kCrLf;
    LF       lf         = LF::kFormatDefault;
    char     indentChar = ' ';
    unsigned indentLen  = 0;

    // Line formatting options
    lf = static_cast<LF>(m_pSetting->lineFormat);

    // End of line options
    switch (m_pSetting->lineEnding)
    {
    case LineEnding::WINDOWS:
        le = LE::kCrLf;
        break;

    case LineEnding::UNIX:
        le = LE::kLf;
        break;

    case LineEnding::MAC:
        le = LE::kCr;
        break;

        // Takes from Notepad++
    case LineEnding::AUTO:
    default:
    {
        const auto eol = m_Editor->GetEOL();
        switch (eol)
        {
        case 0:
            le = LE::kCrLf;
            break;
        case 1:
            le = LE::kCr;
            break;
        default:
            le = LE::kLf;
            break;
        }
    }
    }

    // Indentation options
    switch (m_pSetting->indent.style)
    {
    case IndentStyle::TAB:
        indentChar = '\t';
        indentLen  = 1;
        break;

    case IndentStyle::SPACE:
        indentChar = ' ';
        indentLen  = m_pSetting->indent.len;
        break;

        // Takes from Notepad++
    case IndentStyle::AUTO:
    default:
        auto [c, l] = m_Editor->GetIndent();
        indentChar  = c;
        indentLen   = l;
        break;
    }

    return std::tuple<LE, LF, char, unsigned>(le, lf, indentChar, indentLen);
}

INT_PTR JsonViewDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO *mmi       = reinterpret_cast<MINMAXINFO *>(lParam);
        mmi->ptMinTrackSize.x = m_rcInitialWindowRect.right;
        return 0;
    }

    case WM_SIZE:
    {
        int nHeight = HIWORD(lParam);
        int nWidth  = LOWORD(lParam);
        if (nWidth >= 150 && nHeight >= 100)
            AdjustDocPanelSize(nWidth, nHeight);
        return TRUE;
    }

    case WM_INITDIALOG:
    {
        // Save ourself in GWLP_USERDATA.
        ::SetWindowLongPtr(getHSelf(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        m_hTreeView->OnInit(getHSelf());

        PrepareButtons();

        // Set default node path as JSON
        SetDlgItemText(_hSelf, IDC_EDT_NODEPATH, JSON_ROOT);

        return TRUE;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            // Handle Button events
        case IDC_BTN_REFRESH:
            DrawJsonTree();
            break;

        case IDC_BTN_FORMAT:
            FormatJson();
            break;

        case IDC_BTN_VALIDATE:
            ValidateJson();
            break;

        case IDC_BTN_SEARCH:
            SearchInTree();
            break;

            // Handle context menu entries
        case IDM_COPY_TREEITEM:
            CUtility::CopyToClipboard(CopyName(), _hSelf);
            break;

        case IDM_COPY_NODENAME:
            CUtility::CopyToClipboard(CopyKey(), _hSelf);
            break;

        case IDM_COPY_NODEVALUE:
            CUtility::CopyToClipboard(CopyValue(), _hSelf);
            break;

        case IDM_COPY_NODEPATH:
            CUtility::CopyToClipboard(CopyPath(), _hSelf);
            break;

        case IDM_EXPANDALL:
            ContextMenuExpand(true);
            break;

        case IDM_COLLAPSEALL:
            ContextMenuExpand(false);
            break;
        }
        return TRUE;
    }

    case WM_SHOWWINDOW:
    {
        bool bVisible = wParam;
        ToggleMenuItemState(bVisible);
        return TRUE;
    }

    case WM_CONTEXTMENU:
    {
        ShowContextMenu(CUtility::GetXFromLPARAM(lParam), CUtility::GetYFromLPARAM(lParam));
        return TRUE;
    }

    case WM_NOTIFY:
    {
        HandleTreeEvents(lParam);
        return TRUE;
    }

    default:
        return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
    }
}
