#pragma once


#include <Windows.h>
#include <CommCtrl.h>

struct SliderRange
{
    int m_nDefault = 100;
    int m_nMinZoom = 80;
    int m_nMaxZoom = 250;
};    // namespace SliderRange

class SliderCtrl
{
    HWND              m_hParent       = nullptr;
    HWND              m_hSelf         = nullptr;
    HWND              m_hSelfInfo     = nullptr;
    HWND              m_hTreeView     = nullptr;
    WNDPROC           m_oldSliderProc = nullptr;
    const SliderRange m_sliderRange {};

public:
    explicit SliderCtrl(const SliderRange& sliderRange = {});

    ~SliderCtrl();

    void OnInit(HWND hParent, int sliderID, int sliderInfoID);

    HWND GetSliderHandle() const
    {
        return m_hSelf;
    }

    HWND GetSliderInfoHandle() const
    {
        return m_hSelfInfo;
    }

    auto GetRange() const -> const SliderRange&
    {
        return m_sliderRange;
    }

    auto GetPosition() const -> int;
    void SetPosition(int pos) const;
    void UpdateInfo(int zoomPercentage) const;

private:
    // Static window procedure for the slider
    static LRESULT CALLBACK runWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
