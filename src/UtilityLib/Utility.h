#pragma once

#include <optional>
#include <string>
#include <Windows.h>

class CUtility
{
public:
    CUtility()  = default;
    ~CUtility() = default;

    static std::wstring GetVersion(const std::wstring& filePath);

    static HWND CreateToolTip(HWND hWnd, int nCtrlID, const std::wstring& tooltipText, HINSTANCE hInst = nullptr);

    static float GetDesktopScale(HWND hWnd);

    static short GetXFromLPARAM(LPARAM lp);
    static short GetYFromLPARAM(LPARAM lp);

    static auto GetEditCtrlText(HWND hWnd) -> std::wstring;
    static void SetEditCtrlText(HWND hWnd, const std::wstring& txt);

    static bool GetCheckboxStatus(HWND hWnd);
    static void SetCheckboxStatus(HWND hWnd, bool bCheck);

    static bool DirExist(const std::wstring& dirPath);
    static bool FileExist(const std::wstring& filePath);

    static long FileSize(const std::wstring& filePath);

    static bool CreateDir(const std::wstring& dirPath);

    static bool Copy(const std::wstring& srcFile, const std::wstring& dstFile);

    /// <summary>
    /// Parse string to number safely
    /// </summary>
    /// <param name="str">String to parse</param>
    /// <returns>Optional containing the number, or empty if parsing fails</returns>
    static std::optional<long> GetNumber(const std::wstring& str);
};
