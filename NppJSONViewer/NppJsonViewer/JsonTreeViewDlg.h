#pragma once
#include "DockingDlgInterface.h"
#include "PluginInterface.h"
#include "resource.h"
#include "TreeViewCtrl.h"
#include "ScintillaEditor.h"
#include <string>
#include <memory>
#include <vector>

class JsonTreeViewDlg : public DockingDlgInterface
{
	enum class eButton { eRefresh, eValidate, eFormat, eSearch };
public:
	JsonTreeViewDlg(HINSTANCE hIntance, const NppData& nppData, int nCmdId);
	virtual ~JsonTreeViewDlg();

	void ShowDlg(bool bShow);

private:
	void PrepareButtons();
	void SetIconAndTooltip(eButton ctrlType, const std::wstring& toolTip);

	void AdjustDocPanelSize(int nWidth, int nHeight);

	void DrawJsonTree();

	void ClickJsonTree(LPARAM lParam);
	void ClickJsonTreeItem(HTREEITEM htiNode);
	void ClickJsonTreeItemRight(HTREEITEM htiNode, LPPOINT lppScreen);

	void ContextMenuExpand(bool bExpand);

	int ShowMessage(const std::wstring& title, const std::wstring& msg, int flag, bool bForceShow = false);

	void ToggleMenuItemState(bool bVisible);

	void EnableControls(const std::vector<DWORD>& ids, bool enable);

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

