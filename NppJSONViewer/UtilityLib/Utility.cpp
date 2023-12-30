#include "Utility.h"
#include <CommCtrl.h>
#include <memory>
#include <windowsx.h>
#include <filesystem>
#include <shlobj.h>
#include <commdlg.h>
#include <Shlwapi.h>
#include <algorithm>

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Comctl32.lib")


std::wstring CUtility::GetVersion(const std::wstring &filePath)
{
    std::wstring retVer;

    if (!filePath.empty() && ::PathFileExists(filePath.c_str()))
    {
        DWORD handle     = 0;
        DWORD bufferSize = ::GetFileVersionInfoSize(filePath.c_str(), &handle);

        if (bufferSize > 0)
        {
            auto buffer = std::make_unique<unsigned char[]>(bufferSize);
            ::GetFileVersionInfo(filePath.c_str(), 0, bufferSize, reinterpret_cast<LPVOID>(buffer.get()));

            VS_FIXEDFILEINFO *lpFileInfo = nullptr;
            UINT              cbFileInfo = 0;
            VerQueryValue(buffer.get(), TEXT("\\"), reinterpret_cast<LPVOID *>(&lpFileInfo), &cbFileInfo);
            if (cbFileInfo)
            {
                std::wostringstream os;
                os << ((lpFileInfo->dwFileVersionMS & 0xFFFF0000) >> 16);
                os << '.';
                os << (lpFileInfo->dwFileVersionMS & 0x0000FFFF);
                os << '.';
                os << ((lpFileInfo->dwFileVersionLS & 0xFFFF0000) >> 16);
                os << '.';
                os << (lpFileInfo->dwFileVersionLS & 0x0000FFFF);

                retVer = os.str();
            }
        }
    }

    return retVer;
}

HWND CUtility::CreateToolTip(HWND hWnd, int nCtrlID, const std::wstring &tooltipText, HINSTANCE hInst)
{
    if (nCtrlID == 0 || hWnd == nullptr || tooltipText.empty())
    {
        return nullptr;
    }

    // Get the window of the tool.
    HWND hControl = GetDlgItem(hWnd, nCtrlID);

    // Create the tooltip. g_hInst is the global instance handle.
    HWND hToolTip
        = CreateWindowEx(NULL, TOOLTIPS_CLASS, nullptr, WS_POPUP | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, nullptr, hInst, nullptr);

    if (hControl == nullptr || hToolTip == nullptr)
    {
        return nullptr;
    }

    // Associate the tooltip with the control.
    TOOLINFO toolInfo = {};
    toolInfo.cbSize   = sizeof(toolInfo);
    toolInfo.hwnd     = hWnd;
    toolInfo.uFlags   = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId      = reinterpret_cast<UINT_PTR>(hControl);
    toolInfo.lpszText = const_cast<wchar_t *>(tooltipText.c_str());
    SendMessage(hToolTip, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&toolInfo));

    return hToolTip;
}

float CUtility::GetDesktopScale(HWND hWnd)
{
    HDC   hdc    = GetDC(hWnd);
    float fScale = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
    ReleaseDC(hWnd, hdc);
    return fScale;
}

short CUtility::GetXFromLPARAM(LPARAM lp)
{
    return static_cast<short>(LOWORD(lp));
}

short CUtility::GetYFromLPARAM(LPARAM lp)
{
    return static_cast<short>(HIWORD(lp));
}

std::wstring CUtility::GetEditCtrlText(HWND hWnd)
{
    auto                 length = Edit_GetTextLength(hWnd);
    std::vector<wchar_t> buf(length + 1);
    Edit_GetText(hWnd, buf.data(), static_cast<int>(buf.size()));
    return buf.data();
}

void CUtility::SetEditCtrlText(HWND hWnd, const std::wstring &txt)
{
    Edit_SetText(hWnd, txt.data());
}

bool CUtility::GetCheckboxStatus(HWND hWnd)
{
    return Button_GetCheck(hWnd);
}

void CUtility::SetCheckboxStatus(HWND hWnd, bool bCheck)
{
    Button_SetCheck(hWnd, bCheck);
}

bool CUtility::DirExist(const std::wstring &dirPath)
{
    return std::filesystem::exists(dirPath);
}

bool CUtility::FileExist(const std::wstring &filePath)
{
    bool r = std::filesystem::exists(filePath);
    return r;
}

long CUtility::FileSize(const std::wstring &filePath)
{
    auto r = std::filesystem::file_size(filePath);
    return static_cast<long>(r);
}

bool CUtility::CreateDir(const std::wstring &dirPath)
{
    bool r = std::filesystem::create_directories(dirPath);
    return r;
}

bool CUtility::Copy(const std::wstring &srcFile, const std::wstring &dstFile)
{
    bool r = std::filesystem::copy_file(srcFile, dstFile);
    return r;
}

auto CUtility::GetFileName(const std::wstring &fullPath, bool withExtension) -> std::wstring
{
    std::filesystem::path pathObj(fullPath);

    // Check if file name is required without extension
    if (withExtension == false)
    {
        // Check if file has stem i.e. filename without extension
        if (pathObj.has_stem())
        {
            // return the stem (file name without extension) from path object
            return pathObj.stem().wstring();
        }
    }

    // return the file name with extension from path object
    return pathObj.filename().wstring();
}

auto CUtility::GetFileExtension(const std::wstring &fileName) -> std::wstring
{
    std::wstring retVal;

    std::filesystem::path pathObj(fileName);

    // Check if file has stem i.e. filename without extension
    if (pathObj.has_stem())
    {
        retVal = pathObj.filename().extension().wstring();
    }
    return retVal;
}

auto CUtility::GetTempFilePath() -> std::wstring
{
    TCHAR tmpDir[1024];
    GetTempPath(1024, tmpDir);
    return tmpDir;
}

auto CUtility::GetSpecialFolderLocation(int folderKind) -> std::wstring
{
    wchar_t       path[MAX_PATH]        = {};
    const HRESULT specialLocationResult = SHGetFolderPath(nullptr, folderKind, nullptr, SHGFP_TYPE_CURRENT, path);

    std::wstring result;
    if (SUCCEEDED(specialLocationResult))
    {
        result = path;
    }
    return result;
}

bool CUtility::OpenFileDlg(std::wstring &filePath, const std::wstring &dlgTitle, const std::vector<wchar_t> &dlgFilter, DWORD flags)
{
    bool bRetVal = false;

    OPENFILENAME ofn;
    ::memset(&ofn, 0, sizeof(ofn));
    wchar_t fileName[MAX_PATH] = {};
    ofn.lStructSize            = sizeof(ofn);
    ofn.lpstrTitle             = dlgTitle.c_str();
    ofn.lpstrFilter            = dlgFilter.data();
    ofn.nFilterIndex           = 1;
    ofn.lpstrFile              = fileName;
    ofn.nMaxFile               = MAX_PATH;
    ofn.Flags                  = flags ? flags : OFN_FILEMUSTEXIST;

    if (::GetOpenFileName(&ofn) != FALSE)
    {
        filePath = ofn.lpstrFile;    // will have the full path and file name.
        bRetVal  = true;
    }

    return bRetVal;
}

bool CUtility::CopyToClipboard(const std::wstring &str2cpy, HWND hWnd)
{
    size_t  len2Allocate = (str2cpy.size() + 1) * sizeof(TCHAR);
    HGLOBAL hglbCopy     = ::GlobalAlloc(GMEM_MOVEABLE, len2Allocate);
    if (hglbCopy == NULL)
    {
        return false;
    }

    if (!::OpenClipboard(hWnd))
    {
        ::GlobalFree(hglbCopy);
        ::CloseClipboard();
        return false;
    }

    if (!::EmptyClipboard())
    {
        ::GlobalFree(hglbCopy);
        ::CloseClipboard();
        return false;
    }

    // Lock the handle and copy the text to the buffer.
    wchar_t *pStr = reinterpret_cast<wchar_t *>(::GlobalLock(hglbCopy));
    if (pStr == NULL)
    {
        ::GlobalUnlock(hglbCopy);
        ::GlobalFree(hglbCopy);
        ::CloseClipboard();
        return false;
    }

    wcscpy_s(pStr, len2Allocate / sizeof(wchar_t), str2cpy.c_str());
    ::GlobalUnlock(hglbCopy);

    // Place the handle on the clipboard.
    unsigned int clipBoardFormat = CF_UNICODETEXT;
    if (::SetClipboardData(clipBoardFormat, hglbCopy) == NULL)
    {
        ::GlobalFree(hglbCopy);
        ::CloseClipboard();
        return false;
    }

    if (!::CloseClipboard())
    {
        return false;
    }

    return true;
}

bool CUtility::IsNumber(const std::wstring &str)
{
	return !str.empty() && std::find_if(str.begin(),
		str.end(), [](wchar_t c) { return !std::isdigit(c); }) == str.end();
}

auto CUtility::GetNumber(const std::wstring &str) -> std::optional<int>
{
    std::optional<int> retVal = std::nullopt;
    if (IsNumber(str))
        retVal = std::make_optional<int>(std::stoi(str));

    return retVal;
}
