#ifndef JSONDIALOG_H
#define JSONDIALOG_H

#include "DockingDlgInterface.h"
#include "PluginInterface.h"
#include "json.h"

class JSONDialog : public DockingDlgInterface
{
	char *curJSON;
	HANDLE hTree;
	void drawTree();
	HTREEITEM initTree(HWND hWndDlg);
	HTREEITEM insertToTree(HWND hWndDlg,HTREEITEM parent,char *text);
	void populateTree (HWND hWndDlg, HTREEITEM tree_root, json_t * json_root, int level);
public :
	JSONDialog() : DockingDlgInterface(IDD_TREE){};

    virtual void display(bool toShow = true) const {
        DockingDlgInterface::display(toShow);
        /*if (toShow)
			::SetFocus(::GetDlgItem(_hSelf, IDC_TREE1));*/
    };

	void setParent(HWND parent2set){
		_hParent = parent2set;
	};

	void setJSON(char* json)
	{
		curJSON=json;
		drawTree();
	}

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif //JSONDIALOG_H
