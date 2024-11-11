#include "SliderCtrl.h"

#include <string>

SliderCtrl::SliderCtrl(const SliderRange& sliderRange)
    : m_sliderRange(sliderRange)
{
}

SliderCtrl::~SliderCtrl()
{
    // Restore the original window procedure on cleanup
    SetWindowLongPtr(m_hSelf, GWLP_WNDPROC, (LONG_PTR)m_oldSliderProc);
}

void SliderCtrl::OnInit(HWND hParent, int sliderID, int sliderInfoID)
{
    m_hParent   = hParent;
    m_hSelf     = GetDlgItem(m_hParent, sliderID);
    m_hSelfInfo = GetDlgItem(m_hParent, sliderInfoID);

    // Set slider range and initial position
    SendMessage(m_hSelf, TBM_SETRANGE, TRUE, MAKELPARAM(m_sliderRange.m_nMinZoom, m_sliderRange.m_nMaxZoom));
    SendMessage(m_hSelf, TBM_SETPOS, TRUE, m_sliderRange.m_nDefault);

    UpdateInfo(m_sliderRange.m_nDefault);

    // Subclass the slider control to handle double-click events
    m_oldSliderProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_hSelfInfo, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(runWinProc)));

    // Save this instance for access in the static window procedure
    SetWindowLongPtr(m_hSelfInfo, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

auto SliderCtrl::GetPosition() const -> int
{
    int pos = static_cast<int>(SendMessage(m_hSelf, TBM_GETPOS, 0, 0));
    return pos;
}

void SliderCtrl::SetPosition(int pos) const
{
    // Set slider position
    SendMessage(m_hSelf, TBM_SETPOS, TRUE, pos);

    // Set slider position in text value
    UpdateInfo(pos);
}

void SliderCtrl::UpdateInfo(int zoomPercentage) const
{
    std::wstring sliderInfoText = std::to_wstring(zoomPercentage) + L"%";
    SetWindowText(m_hSelfInfo, sliderInfoText.c_str());
}

LRESULT SliderCtrl::runWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SliderCtrl* pThis = reinterpret_cast<SliderCtrl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (pThis)
    {
        if (message == WM_LBUTTONDBLCLK)
        {
            // Reset slider to 100% on double-click
            // Also notify its parent to adjust tree view as well
            pThis->SetPosition(100);
            SendMessage(pThis->m_hParent, WM_HSCROLL, NULL, reinterpret_cast<LPARAM>(pThis->m_hSelf));
        }

        // Call the original window procedure for other messages
        return CallWindowProc(pThis->m_oldSliderProc, hWnd, message, wParam, lParam);
    }

    return FALSE;
}
