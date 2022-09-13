// This file is part of Notepad++ project
// Copyright (C)2003 Don HO <don.h@free.fr>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// Note that the GPL places important restrictions on "derived works", yet
// it does not provide a detailed definition of that term.  To avoid      
// misunderstandings, we consider an application to constitute a          
// "derivative work" for the purpose of this license if it does any of the
// following:                                                             
// 1. Integrates source code from Notepad++.
// 2. Integrates/includes/aggregates Notepad++ into a proprietary executable
//    installer, such as those produced by InstallShield.
// 3. Links to a library or executes a program that does any of the above.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#pragma once

#include "Window.h"
#include <string>

class URLCtrl : public Window
{
public:
	URLCtrl() = default;

	void create(HWND itemHandle, const TCHAR* link, COLORREF linkColor = RGB(0, 0, 255));
	void create(HWND itemHandle, int cmd, HWND msgDest = NULL);
	void destroy();

private:
	void action();
	COLORREF getCtrlBgColor(HWND hWnd);

protected:
	std::wstring	_URL = TEXT("");
	HFONT			_hfUnderlined = nullptr;
	HCURSOR			_hCursor = nullptr;
	HWND			_msgDest = nullptr;
	unsigned long	_cmdID = 0;

	WNDPROC			_oldproc = nullptr;
	COLORREF		_linkColor;
	COLORREF		_visitedColor;
	bool			_clicking = false;

	static LRESULT CALLBACK URLCtrlProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		return (reinterpret_cast<URLCtrl*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA)))->runProc(hwnd, Message, wParam, lParam);
	}

	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

