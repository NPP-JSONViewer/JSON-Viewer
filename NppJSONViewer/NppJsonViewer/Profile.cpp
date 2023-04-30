#include "Profile.h"
#include "Utility.h"
#include "Define.h"
#include "StringHelper.h"
#include <shlobj.h>
#include <memory>

Profile::Profile(const std::wstring &path)
    : m_ProfileFilePath(path)
{
    if (path.empty())
        Init();
}

bool Profile::ReadValue(const std::wstring &section, const std::wstring &key, int &retVal, int defaultVal) const
{
    retVal = GetPrivateProfileInt(section.c_str(), key.c_str(), defaultVal, m_ProfileFilePath.c_str());

    return true;
}

bool Profile::ReadValue(const std::wstring &section, const std::wstring &key, std::wstring &retVal, const std::wstring &defaultVal) const
{
    bool bRetVal = false;

    // Try with MAX_PATH
    constexpr DWORD nBufSize = MAX_PATH * 2;
    auto            pData    = std::make_unique<TCHAR[]>(nBufSize);
    GetPrivateProfileString(section.c_str(), key.c_str(), defaultVal.c_str(), pData.get(), nBufSize, m_ProfileFilePath.c_str());

    if (pData)
    {
        bRetVal = true;
        retVal  = pData.get();
    }

    return bRetVal;
}

bool Profile::WriteValue(const std::wstring &section, const std::wstring &key, int value) const
{
    return WriteValue(section, key, std::to_wstring(value));
}

bool Profile::WriteValue(const std::wstring &section, const std::wstring &key, const std::wstring &value) const
{
    return WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), m_ProfileFilePath.c_str()) ? true : false;
}

void Profile::Init()
{
    auto appDatapath = CUtility::GetSpecialFolderLocation(CSIDL_APPDATA);
    if (appDatapath.empty())
    {
        ::MessageBox(NULL, L"Failed to get %appdata% path. Please contact developer. Inconvenience regretted.", JSON_ERROR_TITLE, MB_OK | MB_ICONERROR);
        return;
    }

    appDatapath += L"\\config";
    if (!CUtility::DirExist(appDatapath) && !CUtility::CreateDir(appDatapath))
    {
        std::wstring msg = L"Failed to get below directory. Please contact developer. Inconvenience regretted.";
        msg += L"\n\n" + appDatapath;

        ::MessageBox(NULL, msg.c_str(), JSON_ERROR_TITLE, MB_OK | MB_ICONERROR);
        return;
    }

    m_ProfileFilePath = appDatapath + L"\\" + PLUGIN_CONFIG;
}

bool ProfileSetting::GetSettings(Setting &info) const
{
    bool bRetVal = true;

    int nVal = 0;
    bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_EOL, nVal, static_cast<int>(info.lineEnding));
    if (bRetVal)
        info.lineEnding = static_cast<LineEnding>(nVal);

    bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_LINE, nVal, static_cast<int>(info.lineFormat));
    if (bRetVal)
        info.lineFormat = static_cast<LineFormat>(nVal);

    bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENT, nVal, static_cast<int>(info.indent.style));
    if (bRetVal)
        info.indent.style = static_cast<IndentStyle>(nVal);

    bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENTCOUNT, nVal, info.indent.len);
    if (bRetVal)
        info.indent.len = nVal;

    bRetVal &= ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_FOLLOW_TAB, nVal, info.bFollowCurrentTab);
    if (bRetVal)
        info.bFollowCurrentTab = static_cast<bool>(nVal);

    bRetVal &= ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_AUTO_FORMAT, nVal, info.bAutoFormat);
    if (bRetVal)
        info.bAutoFormat = static_cast<bool>(nVal);

    bRetVal &= ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_USE_HIGHLIGHT, nVal, info.bUseJsonHighlight);
    if (bRetVal)
        info.bUseJsonHighlight = static_cast<bool>(nVal);

    bRetVal &= ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMENT, nVal, info.parseOptions.bIgnoreComment);
    if (bRetVal)
        info.parseOptions.bIgnoreComment = static_cast<bool>(nVal);

    bRetVal &= ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMA, nVal, info.parseOptions.bIgnoreTraillingComma);
    if (bRetVal)
        info.parseOptions.bIgnoreTraillingComma = static_cast<bool>(nVal);

    bRetVal &= ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_REPLACE_UNDEFINED, nVal, info.parseOptions.bReplaceUndefined);
    if (bRetVal)
        info.parseOptions.bReplaceUndefined = static_cast<bool>(nVal);

    return bRetVal;
}

bool ProfileSetting::SetSettings(const Setting &info) const
{
    bool bRetVal = true;

    bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_EOL, static_cast<int>(info.lineEnding));
    bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_LINE, static_cast<int>(info.lineFormat));
    bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENT, static_cast<int>(info.indent.style));
    bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENTCOUNT, info.indent.len);

    bRetVal &= WriteValue(STR_INI_OTHER_SEC, STR_INI_OTHER_FOLLOW_TAB, info.bFollowCurrentTab);
    bRetVal &= WriteValue(STR_INI_OTHER_SEC, STR_INI_OTHER_AUTO_FORMAT, info.bAutoFormat);
    bRetVal &= WriteValue(STR_INI_OTHER_SEC, STR_INI_OTHER_USE_HIGHLIGHT, info.bUseJsonHighlight);
    bRetVal &= WriteValue(STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMENT, info.parseOptions.bIgnoreComment);
    bRetVal &= WriteValue(STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMA, info.parseOptions.bIgnoreTraillingComma);
    bRetVal &= WriteValue(STR_INI_OTHER_SEC, STR_INI_OTHER_REPLACE_UNDEFINED, info.parseOptions.bReplaceUndefined);

    return bRetVal;
}
