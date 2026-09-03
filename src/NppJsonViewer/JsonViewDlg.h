#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <optional>

#include "DockingFeature/DockingDlgInterface.h"
#include "PluginInterface.h"
#include "resource.h"
#include "TreeViewCtrl.h"
#include "SliderCtrl.h"
#include "ScintillaEditor.h"
#include "JsonHandler.h"
#include "JsonNode.h"
#include "TreeHandler.h"
#include "TreeExpansion.h"
#include "TreeState.h"


class JsonViewDlg
    : public DockingDlgInterface
    , public TreeHandler
{
    enum class eButton
    {
        eRefresh,
        eValidate,
        eFormat,
        eSearch
    };

    enum class eMethod
    {
        FormatJson,
        GetCompressedJson,
        ParseJson,
        ValidateJson,
        SortJsonByKey
    };

public:
    JsonViewDlg(HINSTANCE hInstance, const NppData& nppData, const bool& isReady, int nCmdId, std::shared_ptr<Setting>& pSetting);
    virtual ~JsonViewDlg();

    void ShowDlg(bool bShow);
    void FormatJson();
    auto FormatJsonDocument() -> bool;    // true = document handled, caller may redraw the tree
    void CompressJson();
    void SortJsonByKey();
    void HandleTabActivated(uptr_t activatedBufferId);
    void HandleFileClosed(uptr_t bufferId);
    void HandleFileOpened();
    void SyncBufferId();
    void RestoreCurrentTabTree();
    void UpdateTitle();

    HTREEITEM InsertToTree(HTREEITEM parent, const std::string& text) override;
    HTREEITEM InsertToTree(HTREEITEM parent, const std::string& text, const Position& pos) override;
    void      AppendNodeCount(HTREEITEM node, unsigned elementCount, bool bArray) override;

private:
    // bSilent suppresses the modal error box reported for an unparsable
    // document; the error is only shown as a node inside the tree. It is used
    // when the tree is drawn on its own (opening a file), where a modal dialog
    // would interrupt the user who never asked for it.
    void DrawJsonTree(bool bPreserveExpansion = false, bool bSilent = false);
    void ReDrawJsonTree(bool bForce = false, bool bPreserveExpansion = false);
    void HighlightAsJson(bool bForcefully = false) const;
    auto PopulateTreeUsingSax(HTREEITEM tree_root, const std::string& jsonText) -> std::optional<std::wstring>;

    void ValidateJson();

    void UpdateNodePath(HTREEITEM htiNode) const;
    void GoToLine(size_t nLineToGo) const;
    void GoToPosition(size_t nLineToGo, size_t nPos, size_t nLen) const;

    void SearchInTree();

    // Expansion/selection state, captured before the tree is rebuilt and
    // re-applied afterwards (see DrawJsonTree(bPreserveExpansion = true)).
    auto CaptureExpansionState() const -> TreeExpansionState;
    void ApplyExpansionState(const TreeExpansionState& state);

    auto CollectExpandedPaths() const -> std::vector<std::wstring>;
    auto GetCurrentSelectedPath() const -> std::vector<std::wstring>;
    void ExpandByPath(const std::vector<std::wstring>& path);
    void SelectByPath(const std::vector<std::wstring>& path);

    // Key of a node as used inside a node path: the raw key without the
    // surrounding quotes ("name" -> name, [0] -> [0]).
    auto GetPathKey(HTREEITEM hti) const -> std::wstring;

    // Resolve a key path (relative to the tree root) back to a node.
    // Returns nullptr when any level of the path cannot be found.
    auto FindNodeByPath(const std::vector<std::wstring>& path) const -> HTREEITEM;

    // Per-tab snapshots. The tree control is a single shared window, so leaving
    // a tab means capturing what it looked like; coming back replays the
    // snapshot instead of parsing the document again.
    void CaptureCurrentTabState();
    void RestoreTabState(uptr_t bufferId);
    void ShowEmptyTree();

    auto CaptureTreeState() const -> TreeState;
    void ApplyTreeState(const TreeState& state);
    void SaveTreeSnapshot();

    auto GetCurrentBufferId() const -> uptr_t;

    auto GetTitleFileName() const -> std::wstring;
    void PrepareButtons();
    void SetIconAndTooltip(eButton ctrlType, const std::wstring& toolTip);

    void AdjustDocPanelSize(int nWidth, int nHeight);

    // Context menu related functions
    void ShowContextMenu(int x, int y);
    void ShowContextMenu(HTREEITEM htiNode, LPPOINT lppScreen);
    void ContextMenuExpand(bool bExpand);

    auto CopyName() const -> std::wstring;
    auto CopyKey() const -> std::wstring;
    auto CopyValue() const -> std::wstring;
    auto CopyPath() const -> std::wstring;

    int  ShowMessage(const std::wstring& title, const std::wstring& msg, int flag, bool bDonotShow = false);
    void ReportError(const Result& result);

    void ToggleMenuItemState(bool bVisible);

    void ShowControls(const std::vector<DWORD>& ids, bool show);
    void EnableControls(const std::vector<DWORD>& ids, bool enable);

    auto GetZoomLevel() const -> int;
    void SetZoomLevel(int pos) const;
    void SetTreeViewZoom(double dwZoomFactor) const;
    void UpdateUIOnZoom(int zoomPercentage) const;
    void HandleZoomOnScroll(WPARAM wParam) const;
    void PersistZoom(int zoomPercentage);

    void HandleTreeEvents(LPARAM lParam) const;

    auto GetFormatSetting() const -> std::tuple<LE, LF, char, unsigned>;

    bool CheckForTokenUndefined(eMethod method, std::string selectedText, Result& res, HTREEITEM tree_root);

    bool IsMultiSelection(const ScintillaData& scintillaData) const;
    auto IsSelectionValidJson(const ScintillaData& scintillaData) const -> std::optional<std::string>;
    void ProcessScintillaData(const ScintillaData& scintillaData, std::string& text, ScintillaCode& code) const;

protected:
    virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) override;

private:
    int         m_nDlgId      = -1;
    NppData     m_NppData     = {};
    HICON       m_hBtnIcon[4] = {};
    const bool& m_IsNppReady;

    // To handle doc panel resizing
    LONG m_lfDeltaWidth          = 0;
    LONG m_lfDeltaHeight         = 0;
    LONG m_lfInitialClientWidth  = 0;
    LONG m_lfInitialClientHeight = 0;
    RECT m_rcInitialWindowRect   = {};

    std::unique_ptr<wchar_t[]>       m_pCurrFileName;
    std::unique_ptr<ScintillaEditor> m_pEditor   = nullptr;
    std::unique_ptr<TreeViewCtrl>    m_pTreeView = nullptr;
    std::unique_ptr<SliderCtrl>      m_pTreeViewZoom = nullptr;
    std::shared_ptr<Setting>         m_pSetting  = nullptr;

    // Per-tab (buffer) tree snapshots: buffer id -> captured tree state
    std::unordered_map<uptr_t, TreeState> m_tabSnapshots;
    uptr_t                                m_nCurrentBufferId = 0;
};
