#include "Profile.h"
#include "Utility.h"
#include "Define.h"
#include "StringHelper.h"
#include <shlobj.h>
#include <memory>

Profile::Profile(const std::wstring& path)
	: m_ProfileFilePath(path)
{
	if (path.empty())
		Init();
}

bool Profile::ReadValue(const std::wstring& section, const std::wstring& key, int& retVal) const
{
	retVal = GetPrivateProfileInt(section.c_str(), key.c_str(), 0, m_ProfileFilePath.c_str());

	return true;
}

bool Profile::ReadValue(const std::wstring& section, const std::wstring& key, std::wstring& retVal) const
{
	bool bRetVal = false;

	// Try with MAX_PATH
	constexpr DWORD nBufSize = MAX_PATH * 2;
	auto pData = std::make_unique<TCHAR[]>(nBufSize);
	GetPrivateProfileString(section.c_str(), key.c_str(), nullptr, pData.get(), nBufSize, m_ProfileFilePath.c_str());

	if (pData)
	{
		bRetVal = true;
		retVal = pData.get();
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

bool ProfileSetting::GetSettings(Setting& info) const
{
	bool bRetVal = true;

	int nVal = 0;
	bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_EOL, nVal);
	if (bRetVal)
		info.le = static_cast<LineEnding>(nVal);

	bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_LINE, nVal);
	if (bRetVal)
		info.lf = static_cast<LineFormat>(nVal);

	bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENT, nVal);
	if (bRetVal)
		info.indent.style = static_cast<IndentStyle>(nVal);

	bRetVal &= ReadValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENTCOUNT, nVal);
	if (bRetVal)
		info.indent.len = nVal;

	return bRetVal;
}

bool ProfileSetting::SetSettings(const Setting& info) const
{
	bool bRetVal = true;

	bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_EOL, static_cast<int>(info.le));
	bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_LINE, static_cast<int>(info.lf));
	bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENT, static_cast<int>(info.indent.style));
	bRetVal &= WriteValue(STR_INI_FORMATTING_SEC, STR_INI_FORMATTING_INDENTCOUNT, info.indent.len);

	return bRetVal;
}
