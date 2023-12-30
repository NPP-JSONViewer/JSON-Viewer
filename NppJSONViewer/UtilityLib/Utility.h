#pragma once

#include <string>
#include <vector>
#include <Windows.h>
#include <optional>

class CUtility
{
public:
    CUtility()  = default;
    ~CUtility() = default;

    static std::wstring GetVersion(const std::wstring &filePath);

    static HWND CreateToolTip(HWND hWnd, int nCtrlID, const std::wstring &tooltipText, HINSTANCE hInst = nullptr);

    static float GetDesktopScale(HWND hWnd);

    static short GetXFromLPARAM(LPARAM lp);
    static short GetYFromLPARAM(LPARAM lp);

    static auto GetEditCtrlText(HWND hWnd) -> std::wstring;
    static void SetEditCtrlText(HWND hWnd, const std::wstring &txt);

    static bool GetCheckboxStatus(HWND hWnd);
    static void SetCheckboxStatus(HWND hWnd, bool bCheck);

    static bool DirExist(const std::wstring &dirPath);
    static bool FileExist(const std::wstring &filePath);

    static long FileSize(const std::wstring &filePath);

    static bool CreateDir(const std::wstring &dirPath);

    static bool Copy(const std::wstring &srcFile, const std::wstring &dstFile);

    static auto GetFileName(const std::wstring &fullPath, bool withExtension = true) -> std::wstring;
    static auto GetFileExtension(const std::wstring &fileName) -> std::wstring;

    static auto GetTempFilePath() -> std::wstring;
    static auto GetSpecialFolderLocation(int folderKind) -> std::wstring;

    static bool OpenFileDlg(std::wstring &filePath, const std::wstring &dlgTitle, const std::vector<wchar_t> &dlgFilter, DWORD flags = 0);

    static bool CopyToClipboard(const std::wstring &str2cpy, HWND hWnd);

    static bool IsNumber(const std::wstring &str);
    static auto GetNumber(const std::wstring &str) -> std::optional<int>;
};
