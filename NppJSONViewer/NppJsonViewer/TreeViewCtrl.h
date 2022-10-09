#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <string>

class TreeViewCtrl
{
    HWND m_hTree   = nullptr;
    HWND m_hParent = nullptr;

public:
    TreeViewCtrl()  = default;
    ~TreeViewCtrl() = default;

    void OnInit(HWND hParent);

    HWND GetTreeViewHandle()
    {
        return m_hTree;
    }

    auto InitTree() -> HTREEITEM;
    auto InsertNode(const std::wstring &text, LPARAM lparam, HTREEITEM parentNode) -> HTREEITEM;
    void UpdateNodeText(HTREEITEM node, const std::wstring &text);

    bool IsExpanded(HTREEITEM node) const;
    bool IsThisOrAnyChildExpanded(HTREEITEM node) const;
    bool IsThisOrAnyChildCollapsed(HTREEITEM node) const;

    void Expand(HTREEITEM node);
    void Collapse(HTREEITEM node);

    BOOL ScreenToTreeView(LPPOINT lpPoint);
    auto HitTest(LPTVHITTESTINFO lpHTInfo) -> HTREEITEM;

    auto GetRoot() -> HTREEITEM;
    bool SelectItem(HTREEITEM hti, bool firstVisible = false);
    bool HasChild(HTREEITEM hti) const;

    HTREEITEM GetSelection() const;
    void      SetSelection(HTREEITEM hItem) const;

    bool IsItemVisible(HTREEITEM hti);

    HTREEITEM NextItem(HTREEITEM htiCurrent)
    {
        return NextItem(htiCurrent, nullptr);
    }

    HTREEITEM NextItem(HTREEITEM htiCurrent, HTREEITEM htiNextRoot);

    auto GetNodeName(HTREEITEM hti, bool removeTrailingCount) -> std::wstring;
    auto GetNodeKey(HTREEITEM hti) -> std::wstring;
    auto GetNodeValue(HTREEITEM hti) -> std::wstring;
    auto GetNodePath(HTREEITEM hti) -> std::wstring;

private:
    void ExpandOrCollpase(HTREEITEM node, UINT_PTR code);

    HTREEITEM GetParentItem(HTREEITEM hti);

    bool GetTVItem(HTREEITEM hti, TVITEM *tvi) const;
    bool SetTVItem(TVITEM *tvi) const;
};
