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
//#include "json.h"
#include <sstream>
//#include "StopWatch.h"
#include "TreeBuilder.h"
#include "rapidjson/reader.h"

//using win32::Stopwatch;
extern NppData nppData;

/*
Delete all items from the tree and creates the root node
*/
HTREEITEM JSONDialog::initTree(HWND hWndDlg)
{

	int TreeCount=TreeView_GetCount(GetDlgItem(this->getHSelf(),IDC_TREE));
	if(TreeCount>0)
		TreeView_DeleteAllItems(GetDlgItem(this->getHSelf(),IDC_TREE));

	TV_INSERTSTRUCT tvinsert;    

	tvinsert.hParent=NULL;     
	tvinsert.hInsertAfter=TVI_ROOT;
	tvinsert.item.mask=TVIF_TEXT;

	tvinsert.item.pszText=L"JSON";
	HTREEITEM item=(HTREEITEM)SendDlgItemMessage(hWndDlg,IDC_TREE,TVM_INSERTITEM,0,(LPARAM)&tvinsert);

	return item;		
}

/*
inserts a node in the tree
*/
HTREEITEM JSONDialog::insertToTree(HWND hWndDlg,HTREEITEM parent, const char *text)
{
	TV_INSERTSTRUCT tvinsert;    
	HTREEITEM item = NULL;
	tvinsert.hParent=parent;     
	tvinsert.hInsertAfter=TVI_LAST;
	tvinsert.item.mask=TVIF_TEXT;

	auto len = strlen(text) + 1;
	wchar_t *w_msg = new wchar_t[len];
	if (w_msg)
	{
		memset(w_msg, 0, len);
		MultiByteToWideChar(CP_UTF8, NULL, text, -1, w_msg, static_cast<int>(len));

		tvinsert.item.pszText = w_msg;
		item = (HTREEITEM)SendDlgItemMessage(hWndDlg, IDC_TREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
		delete[] w_msg; // fix memory leak
	}

	return item;
}

HTREEITEM JSONDialog::insertToTree(HTREEITEM parent, const char *text) {
	return this->insertToTree(this->getHSelf(), parent, text);
}

void JSONDialog::setJSON(char* json)
{
	curJSON=json;
	if (this->isCreated())
		//drawTree();
		drawTreeSaxParse();
}

void JSONDialog::populateTreeUsingSax(HWND /*hWndDlg*/, HTREEITEM tree_root, char *json)
{
	//Stopwatch sw;
	//sw.Start();
	TreeBuilder handler(this, tree_root);
	rapidjson::Reader reader;
	
	rapidjson::StringStream ss(json);
	if (!reader.Parse<rapidjson::kParseNumbersAsStringsFlag>(ss, handler)) 
	{
		::MessageBox(nppData._nppHandle, TEXT("Could not parse!!"), TEXT("JSON Viewer"), MB_OK | MB_ICONERROR);

		//mark the error position
		// Get the current scintilla
		int which = -1;
		::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
		if (which == -1)
			return;

		HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
		size_t start = ::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0);

		size_t errPosition = start + reader.GetErrorOffset();
		::SendMessage(curScintilla, SCI_SETSEL, errPosition, errPosition + 1);
	}
	
  ::SendMessage(nppData._nppHandle, NPPM_SETCURRENTLANGTYPE, 0, LangType::L_JSON);

	//sw.Stop();
	//long long elapsed = sw.ElapsedMilliseconds();
	//std::wstringstream s;
	//s << "tree_time:" << elapsed << " ms";

	//::MessageBox(nppData._nppHandle, s.str().c_str(), TEXT("JSON Viewer"), MB_OK);
}

//void JSONDialog::populateTree (HWND hWndDlg, HTREEITEM tree_root, json_t * json_root, int level)
//{
//	HTREEITEM newItem = NULL;
//	switch (json_root->type)
//	{
//	case JSON_STRING:
//		/*
//		insert the value with its key in a form "key":"value"
//		*/
//		if(json_root->child==NULL && json_root->parent->type!=JSON_ARRAY)
//		{
//			TVITEM t;
//			t.hItem=tree_root;
//			t.mask=TVIF_HANDLE;
//			if(SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_GETITEM,0,(LPARAM)&t))
//			{
//				int len=strlen(json_root->parent->text)+3+strlen(json_root->text)+1;
//				char *txt=new char[len];
//				if (txt)
//				{
//					memset(txt, 0, len);
//					char *unesc_text = json_unescape(json_root->text);
//					char *unesc_parent_text = json_unescape(json_root->parent->text);
//					sprintf(txt, "%s : %s", unesc_parent_text, unesc_text);
//					free(unesc_text);
//					free(unesc_parent_text);
//
//					len = strlen(txt) + 1;
//					wchar_t *w_txt = new wchar_t[len];
//					if (w_txt)
//					{
//						memset(w_txt, 0, len);
//						MultiByteToWideChar(CP_UTF8, NULL, txt, -1, w_txt, len);
//
//						t.pszText = w_txt;
//						t.mask = TVIF_TEXT;
//						::SendDlgItemMessage(hWndDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&t);
//						delete[] w_txt; //fix memory leak
//					}
//
//					delete[] txt; //fix memory leak
//				}
//			}
//		}else
//		{
//			// it is an array element, insert directly
//			char *unesc_elem=json_unescape(json_root->text);
//			newItem=insertToTree(hWndDlg,tree_root,unesc_elem);
//			free(unesc_elem);
//		}
//		break;
//	case JSON_NUMBER:
//		/*
//		insert the value with its key in a form "key":"value"
//		*/
//		if(json_root->child==NULL && json_root->parent->type!=JSON_ARRAY)
//		{
//			TVITEM t;
//			t.hItem=tree_root;
//			t.mask=TVIF_HANDLE;
//			if(SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_GETITEM,0,(LPARAM)&t))
//			{
//				int len=strlen(json_root->parent->text)+3+strlen(json_root->text)+1;
//				char *txt=new char[len];
//				if (txt)
//				{
//					memset(txt, 0, len);
//					char *unesc_parent_text = json_unescape(json_root->parent->text);
//					sprintf(txt, "%s : %s", unesc_parent_text, json_root->text);
//					free(unesc_parent_text);
//
//					len = strlen(txt) + 1;
//					wchar_t *w_txt = new wchar_t[len];
//					if (w_txt)
//					{
//						memset(w_txt, 0, len);
//						MultiByteToWideChar(CP_UTF8, NULL, txt, -1, w_txt, len);
//
//						t.pszText = w_txt;
//						t.mask = TVIF_TEXT;
//						::SendDlgItemMessage(hWndDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&t);
//
//						delete[] w_txt;// fix memory leak
//					}
//
//					delete[] txt; // fix memory leak
//				}
//			}
//		}else
//		{
//			// it is an array element, insert directly
//			newItem=insertToTree(hWndDlg,tree_root,json_root->text);
//		}
//		break;
//	case JSON_OBJECT:
//		newItem=insertToTree(hWndDlg,tree_root,"Object");
//		break;
//	case JSON_ARRAY:
//		newItem=insertToTree(hWndDlg,tree_root,"Array");
//		break;
//	case JSON_TRUE:
//		/*
//		insert the value with its key in a form "key":"value"
//		*/
//		if(json_root->child==NULL && json_root->parent->type!=JSON_ARRAY)
//		{
//			TVITEM t;
//			t.hItem=tree_root;
//			t.mask=TVIF_HANDLE;
//			if(SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_GETITEM,0,(LPARAM)&t))
//			{
//				int len=strlen(json_root->parent->text)+3+strlen("True")+1;
//				char *txt=new char[len];
//				if (txt)
//				{
//					memset(txt, 0, len);
//					char *unesc_text = json_unescape("True");
//					char *unesc_parent_text = json_unescape(json_root->parent->text);
//					sprintf(txt, "%s : %s", unesc_parent_text, unesc_text);
//					free(unesc_text);
//					free(unesc_parent_text);
//
//					len = strlen(txt) + 1;
//					wchar_t *w_txt = new wchar_t[len];
//					
//					if (w_txt)
//					{
//						memset(w_txt, 0, len);
//						MultiByteToWideChar(CP_UTF8, NULL, txt, -1, w_txt, len);
//
//						t.pszText = w_txt;
//						t.mask = TVIF_TEXT;
//						::SendDlgItemMessage(hWndDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&t);
//						delete[] w_txt;//fix memory leak
//					}
//					delete[] txt; //fix memory leak
//				}
//			}
//		}else
//		{
//			// it is an array element, insert directly
//			char *unesc_elem=json_unescape(json_root->text);
//			newItem=insertToTree(hWndDlg,tree_root,unesc_elem);
//			free(unesc_elem);
//		}
//		break;
//	case JSON_FALSE:
//		/*
//		insert the value with its key in a form "key":"value"
//		*/
//		if(json_root->child==NULL && json_root->parent->type!=JSON_ARRAY)
//		{
//			TVITEM t;
//			t.hItem=tree_root;
//			t.mask=TVIF_HANDLE;
//			if(SendDlgItemMessage(hWndDlg,IDC_TREE1,TVM_GETITEM,0,(LPARAM)&t))
//			{
//				int len=strlen(json_root->parent->text)+3+strlen("False")+1;
//				char *txt=new char[len];
//				if (txt)
//				{
//					memset(txt, 0, len);
//					char *unesc_text = json_unescape("False");
//					char *unesc_parent_text = json_unescape(json_root->parent->text);
//					sprintf(txt, "%s : %s", unesc_parent_text, unesc_text);
//					free(unesc_text);
//					free(unesc_parent_text);
//
//					len = strlen(txt) + 1;
//					wchar_t *w_txt = new wchar_t[len];
//					if (w_txt)
//					{
//						memset(w_txt, 0, len);
//						MultiByteToWideChar(CP_UTF8, NULL, txt, -1, w_txt, len);
//
//						t.pszText = w_txt;
//						t.mask = TVIF_TEXT;
//						::SendDlgItemMessage(hWndDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&t);
//						delete[] w_txt; // fix memory leak
//					}
//					delete[] txt; // fix memory leak
//				}
//			}
//		}else
//		{
//			// it is an array element, insert directly
//			char *unesc_elem=json_unescape(json_root->text);
//			newItem=insertToTree(hWndDlg,tree_root,unesc_elem);
//			free(unesc_elem);
//		}
//		break;
//	case JSON_NULL:
//		newItem=insertToTree(hWndDlg,tree_root,"null");
//		break;
//	}
//	// DFS
//	if (json_root->child != NULL)
//	{
//		json_t *ita;
//		ita = json_root->child;
//
//		while (ita != NULL)
//		{
//			if (newItem)
//			{
//				populateTree(hWndDlg, newItem, ita, level + 1);
//			}
//			ita = ita->next;
//		}
//	}
//}

/*
parses curJSON and draws the tree.
marks the error location in case of a parsing error
*/
void JSONDialog::drawTreeSaxParse()
{
	HTREEITEM tree_root;
	tree_root = initTree(this->getHSelf());

	if (strlen(curJSON) == 0) 
	{
		insertToTree(this->getHSelf(), tree_root, "Error:Please select a JSON String.");
		TreeView_Expand(GetDlgItem(this->getHSelf(), IDC_TREE), tree_root, TVE_EXPAND);
		return;
	}
	
	populateTreeUsingSax(this->getHSelf(), tree_root, curJSON);
	TreeView_Expand(GetDlgItem(this->getHSelf(), IDC_TREE), tree_root, TVE_EXPAND);
}


/*
parses curJSON and draws the tree.
marks the error location in case of a parsing error
*/
//void JSONDialog::drawTree()
//{
//	enum json_error err;
//	struct json_parsing_info *jpi;
//	HTREEITEM tree_root;
//
//	tree_root=initTree(this->getHSelf());
//
//	if(strlen(curJSON)==0){
//		insertToTree(this->getHSelf(),tree_root,"Error:Please select a JSON String.");
//		TreeView_Expand(GetDlgItem(this->getHSelf(),IDC_TREE1),tree_root,TVE_EXPAND);
//		return;
//	}
//
//
//	jpi = (json_parsing_info*)malloc (sizeof (struct json_parsing_info));
//	if (jpi == NULL)
//	{
//		::MessageBox(nppData._nppHandle,TEXT("Could not parse!!"),TEXT("JSON Viewer"),MB_OK|MB_ICONERROR);
//	}
//
//	json_jpi_init (jpi);
//	Stopwatch sw;
//	sw.Start();
//	err = json_parse_fragment (jpi, curJSON);
//	sw.Stop();
//	long long parse_time = sw.ElapsedMilliseconds();
//
//	sw.Reset();
//	sw.Start();
//	if((err == JSON_WAITING_FOR_EOF) || (err == JSON_OK))
//	{
//		populateTree(this->getHSelf(),tree_root,jpi->cursor,0);
//		TreeView_Expand(GetDlgItem(this->getHSelf(),IDC_TREE1),tree_root,TVE_EXPAND);
//		json_free_value(&jpi->cursor);
//		free(jpi);
//	}else
//	{
//		::MessageBox(nppData._nppHandle,TEXT("Could not parse!!"),TEXT("JSON Viewer"),MB_OK|MB_ICONERROR);
//
//		//mark the error position
//		// Get the current scintilla
//		int which = -1;
//		::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
//		if (which == -1)
//			return;
//
//		HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
//		size_t start = ::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0);
//
//		size_t errPosition=start+jpi->char_num;
//
//		::SendMessage(curScintilla,SCI_SETSEL,errPosition,errPosition+1);
//
//		free(jpi);
//	}
//	sw.Stop();
//	long long tree_time = sw.ElapsedMilliseconds();
//	std::wstringstream s;
//	s << "parse_time:" << parse_time<<" ms, tree_time:"<<tree_time<<" ms";
//	
//	::MessageBox(nppData._nppHandle, s.str().c_str(), TEXT("JSON Viewer"), MB_OK | MB_ICONERROR);
//
//
//}

INT_PTR CALLBACK JSONDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int width,height;
	switch (message) 
	{
	case WM_INITDIALOG:
		hTree=GetDlgItem(this->getHSelf(),IDC_TREE);// tree control
		drawTreeSaxParse();
		return TRUE;

	case WM_SIZE:
		width=LOWORD(lParam);
		height=HIWORD(lParam);
		SetWindowPos(GetDlgItem(this->getHSelf(),IDC_TREE),HWND_TOP,0,0,width,height,SWP_SHOWWINDOW);
		return TRUE;

	default :
		return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

