#pragma once
#include <string>
#include "Define.h"

class Profile
{
    std::wstring m_ProfileFilePath;

public:
    explicit Profile(const std::wstring &path);
    virtual ~Profile() = default;

protected:
    bool ReadValue(const std::wstring &section, const std::wstring &key, int &retVal, int defaultVal = 0) const;
    bool ReadValue(const std::wstring &section, const std::wstring &key, std::wstring &retVal, const std::wstring &defaultVal = {}) const;

    bool WriteValue(const std::wstring &section, const std::wstring &key, int value) const;
    bool WriteValue(const std::wstring &section, const std::wstring &key, const std::wstring &value) const;

private:
    void Init();
};


class ProfileSetting : public Profile
{
public:
    explicit ProfileSetting(const std::wstring &path)
        : Profile(path)
    {
    }
    ~ProfileSetting() = default;

    bool GetSettings(Setting &info) const;
    bool SetSettings(const Setting &info) const;
};
