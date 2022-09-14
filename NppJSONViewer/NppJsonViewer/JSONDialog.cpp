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

	tvinsert.item.pszText = const_cast<wchar_t*>(L"JSON");
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
	if (!reader.Parse<
    rapidjson::kParseNumbersAsStringsFlag|rapidjson::kParseCommentsFlag|
    rapidjson::kParseEscapedApostropheFlag|rapidjson::kParseNanAndInfFlag|rapidjson::kParseTrailingCommasFlag
  >(ss, handler)) 
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

