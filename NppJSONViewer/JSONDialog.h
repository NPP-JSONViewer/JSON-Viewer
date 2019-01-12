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

#pragma once

#include "DockingDlgInterface.h"
#include "PluginInterface.h"
#include <commctrl.h>
#include "resource.h"
#include "json.h"

class JSONDialog : public DockingDlgInterface
{
	char *curJSON = nullptr;
	HANDLE hTree = nullptr;
	//void drawTree();
	void drawTreeSaxParse();
	HTREEITEM initTree(HWND hWndDlg);
	HTREEITEM insertToTree(HWND hWndDlg, HTREEITEM parent, const char *text);
	//void populateTree (HWND hWndDlg, HTREEITEM tree_root, json_t * json_root, int level);
	void populateTreeUsingSax(HWND hWndDlg, HTREEITEM tree_root, char * json);
  public :
	JSONDialog() : DockingDlgInterface(IDD_TREEDLG){};

	HTREEITEM insertToTree(HTREEITEM parent, const char *text);

	virtual void display(bool toShow = true) const
	{
		DockingDlgInterface::display(toShow);
	}

	void setParent(HWND parent2set)
	{
		_hParent = parent2set;
	}

	void setJSON(char *json);

protected:
	virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
};
