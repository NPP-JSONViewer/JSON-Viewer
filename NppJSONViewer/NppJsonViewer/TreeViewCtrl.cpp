#include "TreeViewCtrl.h"
#include "Define.h"
#include "resource.h"
#include "StringHelper.h"


void TreeViewCtrl::OnInit(HWND hParent)
{
    m_hParent = hParent;
    m_hTree   = GetDlgItem(m_hParent, IDC_TREE);
}

auto TreeViewCtrl::InitTree() -> HTREEITEM
{
    int TreeCount = TreeView_GetCount(m_hTree);
    if (TreeCount > 0)
        TreeView_DeleteAllItems(m_hTree);

    return InsertNode(JSON_ROOT, -1, TVI_ROOT);
}

auto TreeViewCtrl::InsertNode(const std::wstring &text, LPARAM lparam, HTREEITEM parentNode) -> HTREEITEM
{
    TV_INSERTSTRUCT tvinsert;

    if (parentNode == TVI_ROOT)
    {
        tvinsert.hParent      = NULL;
        tvinsert.hInsertAfter = TVI_ROOT;
    }
    else
    {
        tvinsert.hParent      = parentNode;
        tvinsert.hInsertAfter = TVI_LAST;
    }

    tvinsert.item.mask    = TVIF_HANDLE | TVIF_TEXT | TVIF_PARAM;
    tvinsert.item.pszText = const_cast<LPTSTR>(text.c_str());
    tvinsert.item.lParam  = lparam;

    HTREEITEM item = reinterpret_cast<HTREEITEM>(SendDlgItemMessage(m_hParent, IDC_TREE, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&tvinsert)));

    return item;
}

bool TreeViewCtrl::IsExpanded(HTREEITEM node) const
{
    return TreeView_GetItemState(m_hTree, node, TVIS_EXPANDED) & TVIS_EXPANDED;
}

bool TreeViewCtrl::IsThisOrAnyChildExpanded(HTREEITEM node) const
{
    if (node)
    {
        HTREEITEM root = TreeView_GetRoot(m_hTree);
        if (node != root && IsExpanded(node))
            return true;

        HTREEITEM htiChild = TreeView_GetNextItem(m_hTree, node, TVGN_CHILD);
        while (htiChild)
        {
            if (IsThisOrAnyChildExpanded(htiChild))
                return true;
            htiChild = TreeView_GetNextItem(m_hTree, htiChild, TVGN_NEXT);
        }
    }
    return false;
}

bool TreeViewCtrl::IsThisOrAnyChildCollapsed(HTREEITEM node) const
{
    if (node)
    {
        if (!IsExpanded(node) && HasChild(node))
            return true;

        HTREEITEM htiChild = TreeView_GetNextItem(m_hTree, node, TVGN_CHILD);
        while (htiChild)
        {
            if (IsThisOrAnyChildCollapsed(htiChild))
                return true;
            htiChild = TreeView_GetNextItem(m_hTree, htiChild, TVGN_NEXT);
        }
    }
    return false;
}

void TreeViewCtrl::Expand(HTREEITEM node)
{
    ExpandOrCollpase(node, TVE_EXPAND);
}

void TreeViewCtrl::Collapse(HTREEITEM node)
{
    ExpandOrCollpase(node, TVE_COLLAPSE);
}

void TreeViewCtrl::ExpandOrCollpase(HTREEITEM node, UINT_PTR code)
{
    TreeView_Expand(m_hTree, node, code);
}

BOOL TreeViewCtrl::ScreenToTreeView(LPPOINT lpPoint)
{
    return ScreenToClient(m_hTree, lpPoint);
}

HTREEITEM TreeViewCtrl::HitTest(LPTVHITTESTINFO lpHTInfo)
{
    return TreeView_HitTest(m_hTree, lpHTInfo);
}

HTREEITEM TreeViewCtrl::GetRoot()
{
    return TreeView_GetRoot(m_hTree);
}

bool TreeViewCtrl::SelectItem(HTREEITEM hti, bool firstVisible)
{
    UINT flag = TVGN_CARET;
    if (firstVisible)
        flag = TVGN_FIRSTVISIBLE;
    return TreeView_Select(m_hTree, hti, flag) ? true : false;
}

bool TreeViewCtrl::HasChild(HTREEITEM hti) const
{
    HTREEITEM htiChild = NULL;
    htiChild           = TreeView_GetChild(m_hTree, hti);
    return htiChild ? true : false;
}

auto TreeViewCtrl::GetNodeName(HTREEITEM hti) -> std::wstring
{
    if (!hti)
        return TEXT("");

    TCHAR  textBuffer[MAX_PATH] {};
    TVITEM tvItem {};
    tvItem.hItem      = hti;
    tvItem.mask       = TVIF_TEXT;
    tvItem.pszText    = textBuffer;
    tvItem.cchTextMax = MAX_PATH;
    SendMessage(m_hTree, TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvItem));
    return tvItem.pszText ? tvItem.pszText : TEXT("");
}

auto TreeViewCtrl::GetNodeKey(HTREEITEM hti) -> std::wstring
{
    std::wstring retVal = GetNodeName(hti);

    auto pos = retVal.find(L" : ");
    if (pos != std::wstring::npos)
    {
        retVal = retVal.substr(0, pos);
    }
    return retVal;
}

auto TreeViewCtrl::GetNodeValue(HTREEITEM hti) -> std::wstring
{
    std::wstring retVal = GetNodeName(hti);

    auto pos = retVal.find(L" : ");
    if (pos != std::wstring::npos)
    {
        retVal = retVal.substr(pos + 3);
    }
    return retVal;
}

auto TreeViewCtrl::GetNodePath(HTREEITEM hti) -> std::wstring
{
    std::wstring wstrJsonPath;
    HTREEITEM    hitTravel = hti;
    bool         bArray    = false;

    while (hitTravel != NULL)
    {
        std::wstring nodeKey = GetNodeKey(hitTravel);

        if (!nodeKey.empty())
        {
            // remove " from the beinging and end
            if (nodeKey[0] == TEXT('"'))
                nodeKey.erase(0, 1);
            if (nodeKey[nodeKey.size() - 1] == TEXT('"'))
                nodeKey.pop_back();

            if (wstrJsonPath.empty())
            {
                wstrJsonPath = nodeKey;
            }
            else
            {
                std::wstring separator = TEXT(".");
                if (nodeKey[0] == TEXT('['))
                {
                    bArray       = true;
                    wstrJsonPath = nodeKey + separator + wstrJsonPath;
                }
                else
                {
                    if (bArray)
                    {
                        bArray = false;
                        separator.clear();
                    }
                    wstrJsonPath = nodeKey + separator + wstrJsonPath;
                }
            }
        }

        HTREEITEM htiParent = GetParentItem(hitTravel);
        hitTravel           = htiParent;
    }

    return wstrJsonPath;
}

HTREEITEM TreeViewCtrl::GetSelection() const
{
    return TreeView_GetSelection(m_hTree);
}

bool TreeViewCtrl::IsItemVisible(HTREEITEM hti)
{
    RECT rect = {};
    BOOL ret  = TreeView_GetItemRect(m_hTree, hti, &rect, FALSE);
    if (ret == FALSE || rect.top < 0)
        return false;

    return true;
}

HTREEITEM TreeViewCtrl::GetParentItem(HTREEITEM hti)
{
    return TreeView_GetParent(m_hTree, hti);
}

/*
 * Get next item of current item on the TreeView.
 * If current item has a child (or chidren) item, next will be the first child item.
 * If current item has no child, next will be its sibling item.
 * If current item has no child and no sibling, next will be its parent's (parent's parent's ...) sibling item.
 * If current item's parent is ROOT and has no sibling item, it will return NULL.
 */
HTREEITEM TreeViewCtrl::NextItem(HTREEITEM htiCurrent, HTREEITEM htiNextRoot)
{
    HTREEITEM htiNext = nullptr;

    // Does it has child.
    htiNext = TreeView_GetChild(m_hTree, htiCurrent);
    if (htiNext)
        return htiNext;

    // Has no child. So find its sibling.
    htiNext = TreeView_GetNextSibling(m_hTree, htiCurrent);
    if (htiNext)
        return htiNext;

    // Has no child and no sibling. Find its parent's (parent's parent's ...) sibling.
    HTREEITEM htiParent = htiCurrent;
    while ((htiParent = TreeView_GetParent(m_hTree, htiParent)) != htiNextRoot)
    {
        htiNext = TreeView_GetNextSibling(m_hTree, htiParent);
        if (htiNext)
            return htiNext;
    }

    return nullptr;
}

bool TreeViewCtrl::GetTVItem(HTREEITEM hti, TCHAR *buf, int bufSize, TVITEM *tvi)
{
    tvi->mask       = TVIF_HANDLE | TVIF_TEXT | TVIF_PARAM;
    tvi->cchTextMax = bufSize;
    tvi->pszText    = buf;
    tvi->hItem      = hti;
    tvi->lParam     = -1;

    return TreeView_GetItem(m_hTree, tvi) ? true : false;
}
