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
    void DrawJsonTree();
    void ReDrawJsonTree(bool bForce = false);
    void HighlightAsJson(bool bForcefully = false) const;
    auto PopulateTreeUsingSax(HTREEITEM tree_root, const std::string& jsonText) -> std::optional<std::wstring>;

    void ValidateJson();

    void UpdateNodePath(HTREEITEM htiNode) const;
    void GoToLine(size_t nLineToGo) const;
    void GoToPosition(size_t nLineToGo, size_t nPos, size_t nLen) const;

    void SearchInTree();

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
