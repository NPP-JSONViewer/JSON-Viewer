#pragma once

#include <string>
#include <memory>
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
    void CompressJson();
    void SortJsonByKey();
    void HandleTabActivated();
    void UpdateTitle();

    HTREEITEM InsertToTree(HTREEITEM parent, const std::string& text) override;
    HTREEITEM InsertToTree(HTREEITEM parent, const std::string& text, const Position& pos) override;
    void      AppendNodeCount(HTREEITEM node, unsigned elementCount, bool bArray) override;

private:
    /// <summary>
    /// Draw the JSON tree structure in the tree view
    /// </summary>
    void DrawJsonTree();

    /// <summary>
    /// Redraw tree with optional force refresh
    /// </summary>
    /// <param name="bForce">Force redraw even if data hasn't changed</param>
    void ReDrawJsonTree(bool bForce = false);

    /// <summary>
    /// Apply JSON syntax highlighting to editor
    /// </summary>
    /// <param name="bForcefully">Force highlighting even if already highlighted</param>
    void HighlightAsJson(bool bForcefully = false) const;

    /// <summary>
    /// Populate tree using SAX parser for streaming large files
    /// </summary>
    /// <param name="tree_root">Root item in tree view</param>
    /// <param name="jsonText">JSON text to parse</param>
    /// <returns>Error message if parsing fails, empty optional if successful</returns>
    auto PopulateTreeUsingSax(HTREEITEM tree_root, const std::string& jsonText) -> std::optional<std::wstring>;

    /// <summary>
    /// Validate current JSON and display any errors
    /// </summary>
    void ValidateJson();

    /// <summary>
    /// Update the node path display for current selection
    /// </summary>
    /// <param name="htiNode">Tree node item</param>
    void UpdateNodePath(HTREEITEM htiNode) const;

    /// <summary>
    /// Navigate to specific line in editor
    /// </summary>
    /// <param name="nLineToGo">Line number to go to</param>
    void GoToLine(size_t nLineToGo) const;

    /// <summary>
    /// Navigate to specific position in editor
    /// </summary>
    /// <param name="nLineToGo">Line number</param>
    /// <param name="nPos">Column position</param>
    /// <param name="nLen">Length of selection</param>
    void GoToPosition(size_t nLineToGo, size_t nPos, size_t nLen) const;

    /// <summary>
    /// Perform search operation in JSON tree
    /// </summary>
    void SearchInTree();

    /// <summary>
    /// Get formatted title for this dialog
    /// </summary>
    /// <returns>Title string with file information</returns>
    auto GetTitleFileName() const -> std::wstring;

    /// <summary>
    /// Initialize toolbar buttons
    /// </summary>
    void PrepareButtons();

    /// <summary>
    /// Set icon and tooltip for toolbar button
    /// </summary>
    /// <param name="ctrlType">Button type identifier</param>
    /// <param name="toolTip">Tooltip text to display</param>
    void SetIconAndTooltip(eButton ctrlType, const std::wstring& toolTip);

    /// <summary>
    /// Adjust document panel size based on window dimensions
    /// </summary>
    /// <param name="nWidth">New panel width</param>
    /// <param name="nHeight">New panel height</param>
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
};
