#pragma once
#include "DockingDlgInterface.h"
#include "PluginInterface.h"
#include "resource.h"
#include "TreeViewCtrl.h"
#include "ScintillaEditor.h"
#include <string>
#include <memory>
#include <vector>

class JsonViewDlg : public DockingDlgInterface
{
	enum class eButton { eRefresh, eValidate, eFormat, eSearch };
public:
	JsonViewDlg(HINSTANCE hIntance, const NppData& nppData, int nCmdId);
	virtual ~JsonViewDlg();

	void ShowDlg(bool bShow);
	void FormatJson();
	void CompressJson();

	// TODO : move to treeviewctrl class
	HTREEITEM InsertToTree(HWND hWndDlg, HTREEITEM parent, const char* text);
	HTREEITEM InsertToTree(HTREEITEM parent, const char* text);

private:
	void PrepareButtons();
	void SetIconAndTooltip(eButton ctrlType, const std::wstring& toolTip);

	void AdjustDocPanelSize(int nWidth, int nHeight);

	void DrawJsonTree();
	void PopulateTreeUsingSax(HTREEITEM tree_root, const std::string& jsonText);

	void ValidateJson();

	void UpdateNodePath(HTREEITEM htiNode);

	// Context menu related functions
	void ShowContextMenu(int x, int y);
	void ShowContextMenu(HTREEITEM htiNode, LPPOINT lppScreen);
	void ContextMenuExpand(bool bExpand);

	auto CopyName() const->std::wstring;
	auto CopyKey() const->std::wstring;
	auto CopyValue() const->std::wstring;
	auto CopyPath() const->std::wstring;

	int ShowMessage(const std::wstring& title, const std::wstring& msg, int flag, bool bForceShow = false);

	void ToggleMenuItemState(bool bVisible);

	void ShowControls(const std::vector<DWORD>& ids, bool enable);
	void EnableControls(const std::vector<DWORD>& ids, bool enable);

	void HandleTreeEvents(LPARAM lParam);

protected:
	virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	int				m_nDlgId = -1;
	NppData			m_NppData = {};
	HICON			m_hBtnIcon[4] = {};

	// To handle doc panel resizing
	LONG			m_lfDeltaWidth = 0;
	LONG			m_lfDeltaHeight = 0;
	LONG			m_lfInitialClientWidth = 0;
	LONG			m_lfInitialClientHeight = 0;
	RECT			m_rcInitialWindowRect = {};

	bool			m_isSilent = false;

	std::unique_ptr<ScintillaEditor>	m_Editor = nullptr;
	std::unique_ptr <TreeViewCtrl>		m_hTreeView = nullptr;
};

