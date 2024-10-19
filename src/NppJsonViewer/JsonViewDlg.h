#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>

#include "DockingDlgInterface.h"
#include "PluginInterface.h"
#include "resource.h"
#include "TreeViewCtrl.h"
#include "ScintillaEditor.h"
#include "JsonHandler.h"


class JsonViewDlg : public DockingDlgInterface
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

    HTREEITEM InsertToTree(HTREEITEM parent, const std::string& text);
    void      AppendNodeCount(HTREEITEM node, unsigned elementCount, bool bArray);

private:
    void DrawJsonTree();
    void HighlightAsJson(bool bForcefully = false) const;
    auto PopulateTreeUsingSax(HTREEITEM tree_root, const std::string& jsonText) -> std::optional<std::wstring>;

    void ValidateJson();

    void UpdateNodePath(HTREEITEM htiNode);

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

    void HandleTreeEvents(LPARAM lParam);

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
    std::unique_ptr<TreeViewCtrl>    m_hTreeView = nullptr;
    std::shared_ptr<Setting>         m_pSetting  = nullptr;
};
