#pragma once
#include <string>
#include "Define.h"

class Profile
{
	std::wstring m_ProfileFilePath;

public:
	Profile(const std::wstring& path);
	virtual ~Profile() = default;

protected:
	bool ReadValue(const std::wstring& section, const std::wstring& key, int& retVal) const;
	bool ReadValue(const std::wstring& section, const std::wstring& key, std::wstring& retVal) const;

	bool WriteValue(const std::wstring& section, const std::wstring& key, int value) const;
	bool WriteValue(const std::wstring& section, const std::wstring& key, const std::wstring& value) const;

private:
	void Init();
};


class ProfileSetting : public Profile
{
public:
	ProfileSetting(const std::wstring& path) : Profile(path) {}
	~ProfileSetting() = default;

	bool GetSettings(Setting& info) const;
	bool SetSettings(const Setting& info) const;
};

