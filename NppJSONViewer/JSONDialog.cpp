/*
This file is part of JSONViewer Plugin for Notepad++
Copyright (C)2011 Kapil Ratnani <kapil.ratnani@iiitb.net>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "JSONDialog.h"
#include "PluginDefinition.h"
#include "json.h"

extern NppData nppData;

/*
Delete all items from the tree and creates the root node
*/
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

/*
inserts a node in the tree
*/
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

void JSONDialog::setJSON(char* json)
{
	curJSON=json;
	if(this->isCreated())
		drawTree();
}

void JSONDialog::populateTree (HWND hWndDlg, HTREEITEM tree_root, json_t * json_root, int level)
{
	HTREEITEM newItem;
	switch (json_root->type)
	{
	case JSON_STRING:
		/*
		if this is the leaf node show it in the same level as the parent.
		in other words, it shows a value with its key in a form "key":"value"
		*/
		if(json_root->child==NULL && json_root->parent->type!=JSON_ARRAY)
		{
			TVITEM t;
			t.hItem=tree_root;
			t.mask=TVIF_HANDLE;
			if(SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_GETITEM,0,(LPARAM)&t))
			{
				int len=strlen(json_root->parent->text)+3+strlen(json_root->text)+1;
				char *txt=new char[len];
				memset(txt, 0, len);
				sprintf(txt,"%s : %s",json_root->parent->text,json_root->text);

				len = strlen(txt) + 1;
				wchar_t *w_txt = new wchar_t[len];
				memset(w_txt, 0, len);
				MultiByteToWideChar(CP_ACP, NULL, txt, -1, w_txt, len);

				t.pszText=w_txt;
				t.mask=TVIF_TEXT;
				::SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_SETITEM,0,(LPARAM)&t);
			}
		}else
		{
			//not a leaf insert in to tree
			newItem=insertToTree(hWndDlg,tree_root,json_root->text);
		}
		break;
	case JSON_NUMBER:
		/*
		if this is the leaf node show it in the same level as the parent.
		in other words, it shows a value with its key in a form "key":"value"
		*/
		if(json_root->child==NULL && json_root->parent->type!=JSON_ARRAY)
		{
			TVITEM t;
			t.hItem=tree_root;
			t.mask=TVIF_HANDLE;
			if(SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_GETITEM,0,(LPARAM)&t))
			{
				int len=strlen(json_root->parent->text)+3+strlen(json_root->text)+1;
				char *txt=new char[len];
				memset(txt, 0, len);
				sprintf(txt,"%s : %s",json_root->parent->text,json_root->text);

				len = strlen(txt) + 1;
				wchar_t *w_txt = new wchar_t[len];
				memset(w_txt, 0, len);
				MultiByteToWideChar(CP_ACP, NULL, txt, -1, w_txt, len);

				t.pszText=w_txt;
				t.mask=TVIF_TEXT;
				::SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_SETITEM,0,(LPARAM)&t);
			}
		}else
		{
			//not a leaf insert in to tree
			newItem=insertToTree(hWndDlg,tree_root,json_root->text);
		}
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

/*
parses curJSON and draws the tree.
marks the error location in case of a parsing error
*/
void JSONDialog::drawTree()
{
	enum json_error err;
	struct json_parsing_info *jpi;
	HTREEITEM tree_root;

	tree_root=initTree(this->getHSelf());

	if(strlen(curJSON)==0){
		insertToTree(this->getHSelf(),tree_root,"Error:Please select a JSON String.");
		TreeView_Expand(GetDlgItem(this->getHSelf(),IDC_TREE1),tree_root,TVE_EXPAND);
		return;
	}


	jpi = (json_parsing_info*)malloc (sizeof (struct json_parsing_info));
	if (jpi == NULL)
	{
		::MessageBox(nppData._nppHandle,TEXT("Could not parse!!"),TEXT("JSON Viewer"),MB_OK|MB_ICONERROR);
	}

	json_jpi_init (jpi);

	err = json_parse_fragment (jpi, curJSON);

	if((err == JSON_WAITING_FOR_EOF) || (err == JSON_OK))
	{
		populateTree(this->getHSelf(),tree_root,jpi->cursor,0);
		TreeView_Expand(GetDlgItem(this->getHSelf(),IDC_TREE1),tree_root,TVE_EXPAND);
		json_free_value(&jpi->cursor);
		free(jpi);
	}else
	{
		::MessageBox(nppData._nppHandle,TEXT("Could not parse!!"),TEXT("JSON Viewer"),MB_OK|MB_ICONERROR);

		//mark the error position
		// Get the current scintilla
		int which = -1;
		::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
		if (which == -1)
			return;

		HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
		size_t start = ::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0);

		size_t errPosition=start+jpi->char_num;

		::SendMessage(curScintilla,SCI_SETSEL,errPosition,errPosition+1);

		free(jpi);
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

