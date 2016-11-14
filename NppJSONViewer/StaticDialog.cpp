//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
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

#include <stdio.h>
#include "StaticDialog.h"

void StaticDialog::goToCenter()
{
	RECT rc;
	::GetClientRect(_hParent, &rc);
	POINT center;
	center.x = rc.left + (rc.right - rc.left)/2;
	center.y = rc.top + (rc.bottom - rc.top)/2;
	::ClientToScreen(_hParent, &center);

	int x = center.x - (_rc.right - _rc.left)/2;
	int y = center.y - (_rc.bottom - _rc.top)/2;

	::SetWindowPos(_hSelf, HWND_TOP, x, y, _rc.right - _rc.left, _rc.bottom - _rc.top, SWP_SHOWWINDOW);
}

HGLOBAL StaticDialog::makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate)
{
	// Get Dlg Template resource
	HRSRC  hDialogRC = ::FindResource(_hInst, MAKEINTRESOURCE(dialogID), RT_DIALOG);
	if (!hDialogRC)
		return NULL;

	HGLOBAL  hDlgTemplate = ::LoadResource(_hInst, hDialogRC);
	if (!hDlgTemplate)
		return NULL;

	DLGTEMPLATE *pDlgTemplate = reinterpret_cast<DLGTEMPLATE *>(::LockResource(hDlgTemplate));
	if (!pDlgTemplate)
		return NULL;

	// Duplicate Dlg Template resource
	unsigned long sizeDlg = ::SizeofResource(_hInst, hDialogRC);
	HGLOBAL hMyDlgTemplate = ::GlobalAlloc(GPTR, sizeDlg);
	*ppMyDlgTemplate = reinterpret_cast<DLGTEMPLATE *>(::GlobalLock(hMyDlgTemplate));

	::memcpy(*ppMyDlgTemplate, pDlgTemplate, sizeDlg);

	DLGTEMPLATEEX *pMyDlgTemplateEx = reinterpret_cast<DLGTEMPLATEEX *>(*ppMyDlgTemplate);
	if (pMyDlgTemplateEx->signature == 0xFFFF)
		pMyDlgTemplateEx->exStyle |= WS_EX_LAYOUTRTL;
	else
		(*ppMyDlgTemplate)->dwExtendedStyle |= WS_EX_LAYOUTRTL;

	return hMyDlgTemplate;
}

void StaticDialog::create(int dialogID, bool isRTL)
{
	if (isRTL)
	{
		DLGTEMPLATE *pMyDlgTemplate = NULL;
		HGLOBAL hMyDlgTemplate = makeRTLResource(dialogID, &pMyDlgTemplate);
		_hSelf = ::CreateDialogIndirectParam(_hInst, pMyDlgTemplate, _hParent, dlgProc, reinterpret_cast<LPARAM>(this));
		::GlobalFree(hMyDlgTemplate);
	}
	else
		_hSelf = ::CreateDialogParam(_hInst, MAKEINTRESOURCE(dialogID), _hParent, dlgProc, reinterpret_cast<LPARAM>(this));

	if (!_hSelf)
	{
		DWORD err = ::GetLastError();
		char errMsg[256];
		sprintf(errMsg, "CreateDialogParam() return NULL.\rGetLastError() == %u", err);
		::MessageBoxA(NULL, errMsg, "In StaticDialog::create()", MB_OK);
		return;
	}

	// if the destination of message NPPM_MODELESSDIALOG is not its parent, then it's the grand-parent
	::SendMessage(_hParent, NPPM_MODELESSDIALOG, MODELESSDIALOGADD, reinterpret_cast<WPARAM>(_hSelf));
}

INT_PTR CALLBACK StaticDialog::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			StaticDialog *pStaticDlg = reinterpret_cast<StaticDialog *>(lParam);
			pStaticDlg->_hSelf = hwnd;
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, static_cast<LONG_PTR>(lParam));
			::GetWindowRect(hwnd, &(pStaticDlg->_rc));
			pStaticDlg->run_dlgProc(message, wParam, lParam);

			return TRUE;
		}

		default:
		{
			StaticDialog *pStaticDlg = reinterpret_cast<StaticDialog *>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (!pStaticDlg)
				return FALSE;
			return pStaticDlg->run_dlgProc(message, wParam, lParam);
		}
	}
}

void StaticDialog::alignWith(HWND handle, HWND handle2Align, PosAlign pos, POINT & point)
{
	RECT rc, rc2;
	::GetWindowRect(handle, &rc);

	point.x = rc.left;
	point.y = rc.top;

	switch (pos)
	{
		case PosAlign::left:
		{
			::GetWindowRect(handle2Align, &rc2);
			point.x -= rc2.right - rc2.left;
			break;
		}
		case PosAlign::right:
		{
			::GetWindowRect(handle, &rc2);
			point.x += rc2.right - rc2.left;
			break;
		}
		case PosAlign::top:
		{
			::GetWindowRect(handle2Align, &rc2);
			point.y -= rc2.bottom - rc2.top;
			break;
		}
		case PosAlign::bottom:
		{
			::GetWindowRect(handle, &rc2);
			point.y += rc2.bottom - rc2.top;
			break;
		}
	}

	::ScreenToClient(_hSelf, &point);
}

