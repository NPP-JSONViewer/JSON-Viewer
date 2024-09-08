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


#include "URLCtrl.h"

void URLCtrl::create(HWND itemHandle, const TCHAR* link, COLORREF linkColor)
{
	// turn on notify style
	::SetWindowLongPtr(itemHandle, GWL_STYLE, ::GetWindowLongPtr(itemHandle, GWL_STYLE) | SS_NOTIFY);

	// set the URL text (not the display text)
	if (link)
		_URL = link;

	// set the hyperlink colour
	_linkColor = linkColor;

	// set the visited colour
	_visitedColor = RGB(128, 0, 128);

	// subclass the static control
	_oldproc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(itemHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(URLCtrlProc)));

	// associate the URL structure with the static control
	::SetWindowLongPtr(itemHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	// save hwnd
	_hSelf = itemHandle;
}
void URLCtrl::create(HWND itemHandle, int cmd, HWND msgDest)
{
	// turn on notify style
	::SetWindowLongPtr(itemHandle, GWL_STYLE, ::GetWindowLongPtr(itemHandle, GWL_STYLE) | SS_NOTIFY);

	_cmdID = cmd;
	_msgDest = msgDest;

	// set the hyperlink colour
	_linkColor = RGB(0, 0, 255);

	// subclass the static control
	_oldproc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(itemHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(URLCtrlProc)));

	// associate the URL structure with the static control
	::SetWindowLongPtr(itemHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	// save hwnd
	_hSelf = itemHandle;
}

void URLCtrl::destroy()
{
	if (_hfUnderlined)
		::DeleteObject(_hfUnderlined);
	if (_hCursor)
		::DestroyCursor(_hCursor);
}

void URLCtrl::action()
{
	if (_cmdID)
	{
		::SendMessage(_msgDest ? _msgDest : _hParent, WM_COMMAND, _cmdID, 0);
	}
	else
	{
		_linkColor = _visitedColor;

		::InvalidateRect(_hSelf, 0, 0);
		::UpdateWindow(_hSelf);

		// Open a browser
		if (_URL != TEXT(""))
		{
			::ShellExecute(NULL, TEXT("open"), _URL.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
		else
		{
			TCHAR szWinText[MAX_PATH];
			::GetWindowText(_hSelf, szWinText, MAX_PATH);
			::ShellExecute(NULL, TEXT("open"), szWinText, NULL, NULL, SW_SHOWNORMAL);
		}
	}
}

COLORREF URLCtrl::getCtrlBgColor(HWND hWnd)
{
	COLORREF crRet = CLR_INVALID;
	if (hWnd && IsWindow(hWnd))
	{
		RECT rc;
		if (GetClientRect(hWnd, &rc))
		{
			HDC hDC = GetDC(hWnd);
			if (hDC)
			{
				HDC hdcMem = CreateCompatibleDC(hDC);
				if (hdcMem)
				{
					HBITMAP hBmp = CreateCompatibleBitmap(hDC,
						rc.right, rc.bottom);
					if (hBmp)
					{
						HGDIOBJ hOld = SelectObject(hdcMem, hBmp);
						if (hOld)
						{
							if (SendMessage(hWnd, WM_ERASEBKGND, reinterpret_cast<WPARAM>(hdcMem), 0))
							{
								crRet = GetPixel(hdcMem, 2, 2); // 0, 0 is usually on the border
							}
							SelectObject(hdcMem, hOld);
						}
						DeleteObject(hBmp);
					}
					DeleteDC(hdcMem);
				}
				ReleaseDC(hWnd, hDC);
			}
		}
	}
	return crRet;
}

LRESULT URLCtrl::runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		// Free up the structure we allocated
	case WM_NCDESTROY:
		//HeapFree(GetProcessHeap(), 0, url);
		break;

		// Paint the static control using our custom
		// colours, and with an underline text style
	case WM_PAINT:
	{
		DWORD dwStyle = static_cast<DWORD>(::GetWindowLongPtr(hwnd, GWL_STYLE));
		DWORD dwDTStyle = DT_SINGLELINE;

		//Test if centered horizontally or vertically
		if (dwStyle & SS_CENTER)	     dwDTStyle |= DT_CENTER;
		if (dwStyle & SS_RIGHT)		 dwDTStyle |= DT_RIGHT;
		if (dwStyle & SS_CENTERIMAGE) dwDTStyle |= DT_VCENTER;

		RECT		rect;
		::GetClientRect(hwnd, &rect);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hwnd, &ps);

		::SetTextColor(hdc, _linkColor);

		::SetBkColor(hdc, getCtrlBgColor(GetParent(hwnd))); ///*::GetSysColor(COLOR_3DFACE)*/);

		// Create an underline font
		if (_hfUnderlined == 0)
		{
			// Get the default GUI font
			LOGFONT lf;
			HFONT hf = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);

			// Add UNDERLINE attribute
			GetObject(hf, sizeof lf, &lf);
			lf.lfUnderline = TRUE;

			// Create a new font
			_hfUnderlined = ::CreateFontIndirect(&lf);
		}

		HANDLE hOld = SelectObject(hdc, _hfUnderlined);

		// Draw the text!
		TCHAR szWinText[MAX_PATH];
		::GetWindowText(hwnd, szWinText, MAX_PATH);
		::DrawText(hdc, szWinText, -1, &rect, dwDTStyle);

		::SelectObject(hdc, hOld);

		::EndPaint(hwnd, &ps);

		return 0;
	}

	case WM_SETTEXT:
	{
		LRESULT ret = ::CallWindowProc(_oldproc, hwnd, Message, wParam, lParam);
		::InvalidateRect(hwnd, 0, 0);
		return ret;
	}
	// Provide a hand cursor when the mouse moves over us
	case WM_SETCURSOR:
	case WM_MOUSEMOVE:
	{
		if (_hCursor == 0)
			_hCursor = LoadCursor(NULL, IDC_HAND);

		SetCursor(_hCursor);
		return TRUE;
	}

	case WM_LBUTTONDOWN:
		_clicking = true;
		break;

	case WM_LBUTTONUP:
		if (_clicking)
		{
			_clicking = false;

			action();
		}

		break;

		//Support using space to activate this object
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
			_clicking = true;
		break;

	case WM_KEYUP:
		if (wParam == VK_SPACE && _clicking)
		{
			_clicking = false;

			action();
		}
		break;

		// A standard static control returns HTTRANSPARENT here, which
		// prevents us from receiving any mouse messages. So, return
		// HTCLIENT instead.
	case WM_NCHITTEST:
		return HTCLIENT;
	}
	return ::CallWindowProc(_oldproc, hwnd, Message, wParam, lParam);
}
