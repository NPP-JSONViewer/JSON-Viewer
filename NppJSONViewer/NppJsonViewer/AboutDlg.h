#pragma once
#include "StaticDialog.h"

class AboutDlg : public StaticDialog
{
public:
	AboutDlg(HINSTANCE hIntance, HWND hParent, int nCmdId);
	~AboutDlg() = default;

	bool ShowDlg(bool bShow);

protected:
	virtual INT_PTR CALLBACK run_dlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void SetVersion(HWND hWnd);

private:
	int			m_nCmdId = -1;
};

