#include <shlobj.h>
#include <memory>

#include "Profile.h"
#include "Utility.h"
#include "Define.h"
#include "StringHelper.h"


Profile::Profile(const std::wstring& path)
    : m_ProfileFilePath(path)
{
    if (path.empty())
        Init();
}

bool Profile::ReadValue(const std::wstring& section, const std::wstring& key, int& retVal, int defaultVal) const
{
    retVal = GetPrivateProfileInt(section.c_str(), key.c_str(), defaultVal, m_ProfileFilePath.c_str());

    return true;
}

bool Profile::ReadValue(const std::wstring& section, const std::wstring& key, std::wstring& retVal, const std::wstring& defaultVal) const
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

bool Profile::WriteValue(const std::wstring& section, const std::wstring& key, int value) const
{
    return WriteValue(section, key, std::to_wstring(value));
}

bool Profile::WriteValue(const std::wstring& section, const std::wstring& key, const std::wstring& value) const
{
    return WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), m_ProfileFilePath.c_str()) ? true : false;
}

void Profile::Init()
{
    auto appDataPath = CUtility::GetSpecialFolderLocation(CSIDL_APPDATA);
    if (appDataPath.empty())
    {
        ::MessageBox(NULL, L"Failed to get %appdata% path. Please contact developer. Inconvenience regretted.", JSON_ERROR_TITLE, MB_OK | MB_ICONERROR);
        return;
    }

    appDataPath += L"\\config";
    if (!CUtility::DirExist(appDataPath) && !CUtility::CreateDir(appDataPath))
    {
        std::wstring msg = L"Failed to get below directory. Please contact developer. Inconvenience regretted.";
        msg += L"\n\n" + appDataPath;

        ::MessageBox(NULL, msg.c_str(), JSON_ERROR_TITLE, MB_OK | MB_ICONERROR);
        return;
    }

    m_ProfileFilePath = appDataPath + L"\\" + PLUGIN_CONFIG;
}

bool ProfileSetting::GetSettings(Setting& info) const
{
    bool bRetVal = true;

    int nVal = 0;
    bRetVal  = bRetVal && ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_EOL, nVal, static_cast<int>(info.lineEnding));
    if (bRetVal)
        info.lineEnding = static_cast<LineEnding>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_LINE, nVal, static_cast<int>(info.lineFormat));
    if (bRetVal)
        info.lineFormat = static_cast<LineFormat>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENT, nVal, static_cast<int>(info.indent.style));
    if (bRetVal)
        info.indent.style = static_cast<IndentStyle>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENTCOUNT, nVal, info.indent.len);
    if (bRetVal)
        info.indent.len = nVal;

    bRetVal = bRetVal && ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_FOLLOW_TAB, nVal, info.bFollowCurrentTab);
    if (bRetVal)
        info.bFollowCurrentTab = static_cast<bool>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_TREE_ZOOM, nVal, info.nTreeZoom);
    if (bRetVal)
        info.nTreeZoom = nVal;

    bRetVal = bRetVal && ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_AUTO_FORMAT, nVal, info.bAutoFormat);
    if (bRetVal)
        info.bAutoFormat = static_cast<bool>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_USE_HIGHLIGHT, nVal, info.bUseJsonHighlight);
    if (bRetVal)
        info.bUseJsonHighlight = static_cast<bool>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMENT, nVal, info.parseOptions.bIgnoreComment);
    if (bRetVal)
        info.parseOptions.bIgnoreComment = static_cast<bool>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMA, nVal, info.parseOptions.bIgnoreTrailingComma);
    if (bRetVal)
        info.parseOptions.bIgnoreTrailingComma = static_cast<bool>(nVal);

    bRetVal = bRetVal && ReadValue(STR_INI_OTHER_SEC, STR_INI_OTHER_REPLACE_UNDEFINED, nVal, info.parseOptions.bReplaceUndefined);
    if (bRetVal)
        info.parseOptions.bReplaceUndefined = static_cast<bool>(nVal);

    return bRetVal;
}

bool ProfileSetting::SetSettings(const Setting& info) const
{
    Setting current;

    if (!GetSettings(current))
        return false;

    bool bRetVal = true;

    auto writeIfChanged = [&](const auto& oldValue, const auto& newValue, const std::wstring& section, const std::wstring& key)
    {
        if (oldValue != newValue)
            bRetVal = bRetVal && WriteValue(section, key, static_cast<int>(newValue));
    };

    writeIfChanged(current.lineEnding, info.lineEnding, STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_EOL);
    writeIfChanged(current.lineFormat, info.lineFormat, STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_LINE);
    writeIfChanged(current.indent.style, info.indent.style, STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENT);
    writeIfChanged(current.indent.len, info.indent.len, STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENTCOUNT);
    writeIfChanged(current.bFollowCurrentTab, info.bFollowCurrentTab, STR_INI_OTHER_SEC, STR_INI_OTHER_FOLLOW_TAB);
    writeIfChanged(current.nTreeZoom, info.nTreeZoom, STR_INI_OTHER_SEC, STR_INI_OTHER_TREE_ZOOM);
    writeIfChanged(current.bAutoFormat, info.bAutoFormat, STR_INI_OTHER_SEC, STR_INI_OTHER_AUTO_FORMAT);
    writeIfChanged(current.bUseJsonHighlight, info.bUseJsonHighlight, STR_INI_OTHER_SEC, STR_INI_OTHER_USE_HIGHLIGHT);
    writeIfChanged(current.parseOptions.bIgnoreComment, info.parseOptions.bIgnoreComment, STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMENT);
    writeIfChanged(current.parseOptions.bIgnoreTrailingComma, info.parseOptions.bIgnoreTrailingComma, STR_INI_OTHER_SEC, STR_INI_OTHER_IGNORE_COMMA);
    writeIfChanged(current.parseOptions.bReplaceUndefined, info.parseOptions.bReplaceUndefined, STR_INI_OTHER_SEC, STR_INI_OTHER_REPLACE_UNDEFINED);

    return bRetVal;
}
