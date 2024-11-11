#pragma once

#include <string>

#include <Windows.h>
#include <CommCtrl.h>

#include "JsonNode.h"

class TreeViewCtrl
{
    HWND   m_hTree              = nullptr;
    HWND   m_hParent            = nullptr;
    int    m_nCtrlID            = 0;
    size_t m_nMaxNodeTextLength = 0;

public:
    TreeViewCtrl()  = default;
    ~TreeViewCtrl() = default;

    void OnInit(HWND hParent, int ctrlID);

    HWND GetTreeViewHandle() const
    {
        return m_hTree;
    }

    auto InitTree() -> HTREEITEM;
    auto InsertNode(const std::wstring& text, LPARAM lparam, HTREEITEM parentNode) -> HTREEITEM;
    void UpdateNodeText(HTREEITEM node, const std::wstring& text);
    auto GetNodeCount() const -> unsigned int;

    bool IsExpanded(HTREEITEM node) const;
    bool IsThisOrAnyChildExpanded(HTREEITEM node) const;
    bool IsThisOrAnyChildCollapsed(HTREEITEM node) const;

    void Expand(HTREEITEM node) const;
    void Collapse(HTREEITEM node) const;

    BOOL ScreenToTreeView(LPPOINT lpPoint) const;
    auto HitTest(LPTVHITTESTINFO lpHTInfo) const -> HTREEITEM;

    auto GetRoot() const -> HTREEITEM;
    bool SelectItem(HTREEITEM hti, bool firstVisible = false) const;
    bool HasChild(HTREEITEM hti) const;

    HTREEITEM GetSelection() const;
    void      SetSelection(HTREEITEM hItem) const;

    bool IsItemVisible(HTREEITEM hti) const;

    HTREEITEM NextItem(HTREEITEM htiCurrent) const
    {
        return NextItem(htiCurrent, nullptr);
    }

    HTREEITEM NextItem(HTREEITEM htiCurrent, HTREEITEM htiNextRoot) const;

    auto GetNodeName(HTREEITEM hti, bool removeTrailingCount) const -> std::wstring;
    auto GetNodePos(HTREEITEM hti) const -> LPARAM;
    auto GetNodeKey(HTREEITEM hti) const -> std::wstring;
    auto GetNodeValue(HTREEITEM hti) const -> std::wstring;
    auto GetNodePath(HTREEITEM hti) const -> std::wstring;
    auto GetNodePosition(HTREEITEM hti) const -> Position*;

private:
    void ExpandOrCollapse(HTREEITEM node, UINT_PTR code) const;

    HTREEITEM GetParentItem(HTREEITEM hti) const;

    bool GetTVItem(HTREEITEM hti, TVITEM* tvi) const;
    bool SetTVItem(TVITEM* tvi) const;

    void FreeNodeData(HTREEITEM hItem);
    void DeleteAllNodes();
};
