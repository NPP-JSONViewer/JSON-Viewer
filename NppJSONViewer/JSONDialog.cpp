//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "JSONDialog.h"
#include "PluginDefinition.h"
#include "json.h"

extern NppData nppData;

HTREEITEM JSONDialog::initTree(HWND hWndDlg)
{
	
	int TreeCount=TreeView_GetCount(GetDlgItem(this->getHSelf(),IDC_TREE1));
	if(TreeCount>0)
		TreeView_DeleteAllItems(GetDlgItem(this->getHSelf(),IDC_TREE1));

	TV_INSERTSTRUCT tvinsert;    

	tvinsert.hParent=NULL;     
	tvinsert.hInsertAfter=TVI_ROOT;
	tvinsert.item.mask=TVIF_TEXT;
                                 
	tvinsert.item.pszText=L"JSON";
	HTREEITEM item=(HTREEITEM)SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_INSERTITEM,0,(LPARAM)&tvinsert);
	
	return item;		
}

HTREEITEM JSONDialog::insertToTree(HWND hWndDlg,HTREEITEM parent,char *text)
{
		TV_INSERTSTRUCT tvinsert;    

		tvinsert.hParent=parent;     
		tvinsert.hInsertAfter=TVI_LAST;
		tvinsert.item.mask=TVIF_TEXT;

		int len = strlen(text) + 1;
		wchar_t *w_msg = new wchar_t[len];
		memset(w_msg, 0, len);
		MultiByteToWideChar(CP_ACP, NULL, text, -1, w_msg, len);

		tvinsert.item.pszText=w_msg;
		HTREEITEM item=(HTREEITEM)SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_INSERTITEM,0,(LPARAM)&tvinsert);
		
		return item;
}

void JSONDialog::populateTree (HWND hWndDlg, HTREEITEM tree_root, json_t * json_root, int level)
{
	HTREEITEM newItem;
	switch (json_root->type)
	{
	case JSON_STRING:
		newItem=insertToTree(hWndDlg,tree_root,json_root->text);
		break;
	case JSON_NUMBER:
		newItem=insertToTree(hWndDlg,tree_root,json_root->text);
		break;
	case JSON_OBJECT:
		newItem=insertToTree(hWndDlg,tree_root,"Object");
		break;
	case JSON_ARRAY:
		newItem=insertToTree(hWndDlg,tree_root,"Array");
		break;
	case JSON_TRUE:
		newItem=insertToTree(hWndDlg,tree_root,"True");
		break;
	case JSON_FALSE:
		newItem=insertToTree(hWndDlg,tree_root,"False");
		break;
	case JSON_NULL:
		newItem=insertToTree(hWndDlg,tree_root,"null");
		break;
	}

	if (json_root->child != NULL)
	{
		json_t *ita, *itb;
		ita = json_root->child;
		
		while (ita != NULL)
		{
			populateTree(hWndDlg,newItem,ita, level + 1);
			itb = ita->next;
			ita = itb;
		}
	}
}

void JSONDialog::drawTree()
{
	json_t *json_root=NULL;
	json_error err;
	HTREEITEM tree_root;
	tree_root=initTree(this->getHSelf());
	err=json_parse_document(&json_root,curJSON);
	if(err==JSON_OK)
	{
		populateTree(this->getHSelf(),tree_root,json_root,0);
		TreeView_Expand(GetDlgItem(this->getHSelf(),IDC_TREE1),tree_root,TVE_EXPAND);
		json_free_value(&json_root);
	}else
	{
		::MessageBox(nppData._nppHandle,TEXT("Could not parse!!"),TEXT("JSON Viewer"),MB_OK|MB_ICONERROR);
	}
}

BOOL CALLBACK JSONDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int width,height;
	switch (message) 
	{
	case WM_INITDIALOG:
		hTree=GetDlgItem(this->getHSelf(),IDC_TREE1);// tree control
		drawTree();
		return TRUE;
	
	case WM_SIZE:
		width=LOWORD(lParam);
		height=HIWORD(lParam);
		SetWindowPos(GetDlgItem(this->getHSelf(),IDC_TREE1),HWND_TOP,0,0,width,height,SWP_SHOWWINDOW);
		return TRUE;

	default :
		return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

