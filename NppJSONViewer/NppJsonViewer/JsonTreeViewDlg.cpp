#include "JsonTreeViewDlg.h"
#include "Define.h"
#include "Utility.h"
#include "TreeBuilder.h"
#include "ScintillaEditor.h"


JsonTreeViewDlg::JsonTreeViewDlg(HINSTANCE hIntance, const NppData& nppData, int nCmdId) :
	m_NppData(nppData),
	DockingDlgInterface(IDD_TREEDLG),
	m_nDlgId(nCmdId),
	m_Editor(std::make_unique<ScintillaEditor>(nppData)),
	m_hTreeView(std::make_unique<TreeViewCtrl>())
{
	_hParent = nppData._nppHandle;
	_hInst = hIntance;
}

JsonTreeViewDlg::~JsonTreeViewDlg()
{
	for (HICON hIcon : m_hBtnIcon)
	{
		if (hIcon != nullptr)
			DestroyIcon(hIcon);
	}
}

void JsonTreeViewDlg::ShowDlg(bool bShow)
{
	if (!isCreated())
	{
		tTbData	data = { 0 };
		create(&data);

		// Set Initial rect, width and Height
		getWindowRect(m_rcInitialWindowRect);

		RECT rc;
		getClientRect(rc);
		m_lfInitialClientWidth = rc.right - rc.left;
		m_lfInitialClientHeight = rc.bottom - rc.top;

		// define the default docking behaviour
		data.uMask = DWS_DF_CONT_LEFT;

		data.pszModuleName = getPluginFileName();
		data.pszName = const_cast<TCHAR*>(TITLE_JSON_PANEL);

		// the dlgDlg should be the index of funcItem where the current function pointer is
		data.dlgID = m_nDlgId;
		::SendMessage(_hParent, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);

		// Draw json tree now
		DrawJsonTree();
	}

	DockingDlgInterface::display(bShow);
}

void JsonTreeViewDlg::PrepareButtons()
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

void JsonTreeViewDlg::SetIconAndTooltip(eButton ctrlType, const std::wstring& toolTip)
{
	int nCtrlID = 0;
	int iconResID = 0;
	switch (ctrlType)
	{
	case eButton::eRefresh:
		nCtrlID = IDC_BTN_REFRESH;
		iconResID = IDI_ICON_REFRESH;
		break;

	case eButton::eValidate:
		nCtrlID = IDC_BTN_VALIDATE;
		iconResID = IDI_ICON_VALIDATE;
		break;

	case eButton::eFormat:
		nCtrlID = IDC_BTN_FORMAT;
		iconResID = IDI_ICON_FORMAT;
		break;

	case eButton::eSearch:
		nCtrlID = IDC_BTN_SEARCH;
		iconResID = IDI_ICON_SEARCH;
		break;

	default:
		return;
	}

	HWND hWnd = ::GetDlgItem(_hSelf, nCtrlID);
	RECT rc = {};
	GetClientRect(hWnd, &rc);

	int nCtrlType = static_cast<int>(ctrlType);
	int icon_size = min(rc.bottom - rc.top, rc.right - rc.left) * 4 / 5;
	m_hBtnIcon[nCtrlType] = static_cast<HICON>(LoadImage(_hInst, MAKEINTRESOURCE(iconResID), IMAGE_ICON, icon_size, icon_size, 0));
	SendMessage(hWnd, BM_SETIMAGE, static_cast<WPARAM>(IMAGE_ICON), reinterpret_cast<LPARAM>(m_hBtnIcon[nCtrlType]));

	CUtility::CreateToolTip(_hSelf, nCtrlID, toolTip, _hInst);
}

void JsonTreeViewDlg::AdjustDocPanelSize(int nWidth, int nHeight)
{
	// Calculate desktop scale.
	float fDeskScale = CUtility::GetDesktopScale(_hSelf);

	auto newDeltaWidth = nWidth - m_lfInitialClientWidth - 2;	// -2 is used for margin
	auto addWidth = static_cast<int>((newDeltaWidth - m_lfDeltaWidth) * fDeskScale);
	m_lfDeltaWidth = newDeltaWidth;

	auto newDeltaHeight = nHeight - m_lfInitialClientHeight;
	auto addHeight = static_cast<int>((newDeltaHeight - m_lfDeltaHeight) * fDeskScale);
	m_lfDeltaHeight = newDeltaHeight;

	// elements that need to be resized horizontally
	const auto resizeWindowIDs = { IDC_EDT_SEARCH, IDC_TREE };

	// elements that need to be moved
	const auto moveWindowIDs = { IDC_BTN_SEARCH, };

	// elements which requires both resizing and move
	const auto resizeAndmoveWindowIDs = { IDC_EDT_NODEPATH };

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

void JsonTreeViewDlg::DrawJsonTree()
{
	// Disable all buttons and treeView
	std::vector<DWORD> ctrls = { IDC_BTN_REFRESH , IDC_BTN_VALIDATE, IDC_BTN_FORMAT, IDC_BTN_SEARCH, IDC_EDT_SEARCH };
	EnableControls(ctrls, false);

	HTREEITEM rootNode = nullptr;
	rootNode = m_hTreeView->InitTree();

	const std::string txtForParsing = m_Editor->GetJsonText();

	if (txtForParsing.empty())
	{
		m_hTreeView->InsertNode(L"Error: Please select a JSON String.", NULL, rootNode);
	}
	else
	{
		PopulateTreeUsingSax(rootNode, txtForParsing);
	}

	m_hTreeView->Expand(rootNode);

	// Enable all buttons and treeView
	EnableControls(ctrls, true);
}

void JsonTreeViewDlg::PopulateTreeUsingSax(HTREEITEM tree_root, const std::string& jsonText)
{
	TreeBuilder handler(this, tree_root);
	rapidjson::Reader reader;

	rapidjson::StringStream ss(jsonText.c_str());
	if (!reader.Parse< rapidjson::kParseNumbersAsStringsFlag | rapidjson::kParseCommentsFlag |
		rapidjson::kParseEscapedApostropheFlag | rapidjson::kParseNanAndInfFlag | rapidjson::kParseTrailingCommasFlag>(ss, handler))
	{
		::MessageBox(m_NppData._nppHandle, TEXT("Could not parse!!"), TEXT("JSON Viewer"), MB_OK | MB_ICONERROR);

		// Mark the error position
		size_t errPosition = reader.GetErrorOffset();
		m_Editor->MarkErrorPosistion(errPosition);
	}

	m_Editor->SetLangAsJson();
}

HTREEITEM JsonTreeViewDlg::InsertToTree(HWND hWndDlg, HTREEITEM parent, const char* text)
{
	TV_INSERTSTRUCT tvinsert;
	HTREEITEM item = NULL;
	tvinsert.hParent = parent;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT;

	auto len = strlen(text) + 1;
	wchar_t* w_msg = new wchar_t[len];
	if (w_msg)
	{
		memset(w_msg, 0, len);
		MultiByteToWideChar(CP_UTF8, NULL, text, -1, w_msg, static_cast<int>(len));

		tvinsert.item.pszText = w_msg;
		item = (HTREEITEM)SendDlgItemMessage(hWndDlg, IDC_TREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
		delete[] w_msg; // fix memory leak
	}

	return item;
}

HTREEITEM JsonTreeViewDlg::InsertToTree(HTREEITEM parent, const char* text)
{
	return InsertToTree(getHSelf(), parent, text);
}

void JsonTreeViewDlg::ClickJsonTree(LPARAM lParam)
{
	LPNMHDR lpnmh = reinterpret_cast<LPNMHDR>(lParam);
	if (!lpnmh || lpnmh->idFrom != IDC_TREE)
		return; // Not click inside JsonTree

	switch (lpnmh->code)
	{
	case NM_CLICK:
	{
		// Let's play safe here
		if (lpnmh->code == NM_RCLICK)
			break;

		DWORD dwPos = GetMessagePos();
		POINT ptScreen, ptClient;
		ptScreen.x = LOWORD(dwPos);
		ptScreen.y = HIWORD(dwPos);

		ptClient = ptScreen;
		m_hTreeView->ScreenToTreeView(&ptClient);

		TVHITTESTINFO ht = {};
		ht.pt = ptClient;
		HTREEITEM hItem = m_hTreeView->HitTest(&ht);
		if (!hItem)
			return; // No hit

		if (ht.flags & TVHT_ONITEMLABEL)
		{
			// Left click
			ClickJsonTreeItem(hItem);
		}
	}
	break;

	case TVN_SELCHANGED:
	{
		NMTREEVIEW* pnmtv = reinterpret_cast<LPNMTREEVIEW>(lParam);
		HTREEITEM hItem = pnmtv->itemNew.hItem;
		if (hItem && pnmtv->action == TVC_BYKEYBOARD)
		{
			ClickJsonTreeItem(hItem);
		}
	}
	break;
	}
}

void JsonTreeViewDlg::ClickJsonTreeItem(HTREEITEM htiNode)
{
	std::wstring nodePath = m_hTreeView->GetNodePath(htiNode);
	SetDlgItemText(_hSelf, IDC_EDT_NODEPATH, nodePath.c_str());
	//m_hTreeView->GotoScintillaLine(htiNode, m_iSelStartLine);
}

void JsonTreeViewDlg::HandleRightClick(HTREEITEM htiNode, LPPOINT lppScreen)
{
	// Select it
	m_hTreeView->SelectItem(htiNode);

	// Show menu
	if (lppScreen != NULL)
	{
		bool bEnableCopyName = true;
		bool bEnableCopyValue = true;
		bool bEnableCopyPath = true;

		bool bEnableExpand = false;
		bool bEnableCollapse = false;

		if (m_hTreeView->GetRoot() == htiNode)
		{
			bEnableCopyName = false;
			bEnableCopyValue = false;
			bEnableCopyPath = false;
		}

		if (m_hTreeView->HasChild(htiNode))
		{
			bEnableCopyValue = false;
			bEnableCollapse = m_hTreeView->IsThisOrAnyChildExpanded(htiNode);
			bEnableExpand = m_hTreeView->IsThisOrAnyChildCollapsed(htiNode);
		}

		// Create menu
		HMENU hMenuPopup = CreatePopupMenu();
		UINT itemFlag;

		itemFlag = MF_STRING | MF_ENABLED;
		AppendMenu(hMenuPopup, itemFlag, IDM_COPY_TREEITEM, STR_COPY);

		// separator
		AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);

		itemFlag = MF_STRING | (bEnableCopyName ? MF_ENABLED : MF_DISABLED);
		AppendMenu(hMenuPopup, itemFlag, IDM_COPY_NODENAME, STR_COPYNAME);

		itemFlag = MF_STRING | (bEnableCopyValue ? MF_ENABLED : MF_DISABLED);
		AppendMenu(hMenuPopup, itemFlag, IDM_COPY_NODEVALUE, STR_COPYVALUE);

		itemFlag = MF_STRING | (bEnableCopyPath ? MF_ENABLED : MF_DISABLED);;
		AppendMenu(hMenuPopup, itemFlag, IDM_COPY_NODEPATH, STR_COPYPATH);

		// separator
		AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);

		itemFlag = MF_STRING | (bEnableExpand ? MF_ENABLED : MF_DISABLED);;
		AppendMenu(hMenuPopup, itemFlag, IDM_EXPANDALL, STR_EXPANDALL);

		itemFlag = MF_STRING | (bEnableCollapse ? MF_ENABLED : MF_DISABLED);;
		AppendMenu(hMenuPopup, itemFlag, IDM_COLLAPSEALL, STR_COLLAPSEALL);

		// Open menu
		TrackPopupMenu(hMenuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, lppScreen->x, lppScreen->y, 0, _hSelf, NULL);

		// Clean up
		DestroyMenu(hMenuPopup);
	}
}

void JsonTreeViewDlg::ShowContextMenu(int x, int y)
{
	POINT p{
		  .x = x
		, .y = y
	};

	TVHITTESTINFO tvHitInfo{
		  .pt = p
		, .flags = 0
		, .hItem = nullptr
	};

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
			ClickJsonTreeItem(hTreeItem);
			HandleRightClick(hTreeItem, &p);
		}
	}
}

void JsonTreeViewDlg::ContextMenuExpand(bool bExpand)
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

int JsonTreeViewDlg::ShowMessage(const std::wstring& title, const std::wstring& msg, int flag, bool bForceShow)
{
	return (!m_isSilent || bForceShow) ? ::MessageBox(_hParent, msg.c_str(), title.c_str(), flag) : IDOK;
}

void JsonTreeViewDlg::ToggleMenuItemState(bool bVisible)
{
	::SendMessage(_hParent, NPPM_SETMENUITEMCHECK, static_cast<WPARAM>(m_nDlgId), bVisible);
}

void JsonTreeViewDlg::ShowControls(const std::vector<DWORD>& ids, bool enable)
{
	for (auto id : ids)
		ShowWindow(GetDlgItem(getHSelf(), id), enable ? SW_HIDE : SW_HIDE);
}

void JsonTreeViewDlg::EnableControls(const std::vector<DWORD>& ids, bool enable)
{
	for (auto id : ids)
		EnableWindow(GetDlgItem(getHSelf(), id), enable ? TRUE : FALSE);
}

INT_PTR JsonTreeViewDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
		mmi->ptMinTrackSize.x = m_rcInitialWindowRect.right;
		return 0;
	}

	case WM_SIZE:
	{
		int nHeight = HIWORD(lParam);
		int nWidth = LOWORD(lParam);
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

		return TRUE;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BTN_REFRESH:
			DrawJsonTree();
			break;

		case IDC_BTN_SEARCH:
			MessageBox(_hSelf, L"IDC_BTN_SEARCH", L"OK", MB_OK);
			break;

		case IDC_BTN_FORMAT:
			MessageBox(_hSelf, L"IDC_BTN_FORMAT", L"OK", MB_OK);
			break;

		case IDC_BTN_VALIDATE:
			MessageBox(_hSelf, L"IDC_BTN_VALIDATE", L"OK", MB_OK);
			break;

			// context menu entries
		case IDM_COPY_TREEITEM:
			MessageBox(_hSelf, L"IDM_COPY_TREEITEM", L"OK", MB_OK);
			break;

		case IDM_COPY_NODENAME:
			MessageBox(_hSelf, L"IDM_COPY_NODENAME", L"OK", MB_OK);
			break;

		case IDM_COPY_NODEVALUE:
			MessageBox(_hSelf, L"IDM_COPY_NODEVALUE", L"OK", MB_OK);
			break;

		case IDM_COPY_NODEPATH:
			MessageBox(_hSelf, L"IDM_COPY_NODEPATH", L"OK", MB_OK);
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
		ClickJsonTree(lParam);
		return TRUE;
	}

	default:
		// TODO: Temporarily hide controls which are not implemented 
		std::vector<DWORD> toHide = { IDC_BTN_SEARCH, IDC_EDT_SEARCH, IDC_EDT_NODEPATH };
		ShowControls(toHide, false);
		return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}
