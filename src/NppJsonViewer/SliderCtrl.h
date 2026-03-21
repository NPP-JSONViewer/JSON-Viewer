#pragma once

#include <Windows.h>
#include <cassert>

/// <summary>
/// Range configuration for slider control
/// </summary>
struct SliderRange
{
    int minValue = 50;    // Minimum slider value
    int maxValue = 200;   // Maximum slider value
    int initialValue = 100;  // Initial slider position
};

/// <summary>
/// Wrapper for Windows slider control with info display
/// Provides position tracking and display of zoom percentage
/// </summary>
class SliderCtrl
{
    HWND              m_hParent       = nullptr;
    HWND              m_hSelf         = nullptr;
    HWND              m_hSelfInfo     = nullptr;
    HWND              m_hTreeView     = nullptr;
    WNDPROC           m_oldSliderProc = nullptr;
    const SliderRange m_sliderRange {};

public:
    /// <summary>
    /// Initialize slider control with range configuration
    /// </summary>
    /// <param name="sliderRange">Range settings for slider</param>
    explicit SliderCtrl(const SliderRange& sliderRange = {});

    ~SliderCtrl();

    /// <summary>
    /// Initialize the slider control in a dialog
    /// </summary>
    /// <param name="hParent">Parent window handle</param>
    /// <param name="sliderID">Control ID of the slider</param>
    /// <param name="sliderInfoID">Control ID of the info label</param>
    void OnInit(HWND hParent, int sliderID, int sliderInfoID);

    /// <summary>
    /// Get slider window handle
    /// </summary>
    /// <returns>Slider HWND, or nullptr if not initialized</returns>
    HWND GetSliderHandle() const
    {
        return m_hSelf;
    }

    /// <summary>
    /// Get slider info label window handle
    /// </summary>
    /// <returns>Info label HWND, or nullptr if not initialized</returns>
    HWND GetSliderInfoHandle() const
    {
        return m_hSelfInfo;
    }

    /// <summary>
    /// Get configured slider range
    /// </summary>
    /// <returns>Const reference to SliderRange configuration</returns>
    auto GetRange() const -> const SliderRange&
    {
        return m_sliderRange;
    }

    /// <summary>
    /// Get current slider position
    /// </summary>
    /// <returns>Current position value within range</returns>
    auto GetPosition() const -> int;

    /// <summary>
    /// Set slider position
    /// </summary>
    /// <param name="pos">Position value (must be within range)</param>
    void SetPosition(int pos) const;

    /// <summary>
    /// Update info label with zoom percentage
    /// </summary>
    /// <param name="zoomPercentage">Zoom percentage to display</param>
    void UpdateInfo(int zoomPercentage) const;

private:
    /// <summary>
    /// Static window procedure for slider subclassing
    /// </summary>
    static LRESULT CALLBACK runWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
