#include <gtest/gtest.h>

#include <memory>
#include <filesystem>

#include "Profile.h"

namespace ProfileSettingTests
{
    class ProfileSettingEx : public ProfileSetting
    {
    public:
        ProfileSettingEx(const std::wstring &path)
            : ProfileSetting(path)
        {
            // any left over from previous run
            RemoveProfileFile();
        }

        ~ProfileSettingEx()
        {
            RemoveProfileFile();
        }

        bool ReadValue(const std::wstring &section, const std::wstring &key, int &retVal, int defaultVal = 0) const
        {
            return Profile::ReadValue(section, key, retVal, defaultVal);
        }

        bool ReadValue(const std::wstring &section, const std::wstring &key, std::wstring &retVal, const std::wstring &defaultVal = {}) const
        {
            return Profile::ReadValue(section, key, retVal, defaultVal);
        }

        bool WriteValue(const std::wstring &section, const std::wstring &key, int value) const
        {
            return Profile::WriteValue(section, key, value);
        }

        bool WriteValue(const std::wstring &section, const std::wstring &key, const std::wstring &value) const
        {
            return Profile::WriteValue(section, key, value);
        }

    private:
        void RemoveProfileFile()
        {
            std::error_code ec {};
            std::filesystem::remove(m_ProfileFilePath, ec);
        }
    };

    class ProfileTest : public ::testing::Test
    {
    protected:
        std::unique_ptr<ProfileSettingEx> m_pProfile;

        void SetUp() override
        {
            std::wstring exePath;
            wchar_t      path[MAX_PATH];
            DWORD        size = GetModuleFileName(nullptr, path, MAX_PATH);
            if (size)
            {
                exePath    = path;
                size_t pos = exePath.find_last_of(L"\\/");
                if (pos != std::wstring::npos)
                {
                    // Remove the executable name, leave only the directory
                    exePath = exePath.substr(0, pos + 1);
                }
            }
            exePath += L"test_profile.ini";

            m_pProfile = std::make_unique<ProfileSettingEx>(exePath);
        }

        void TearDown() override {}
    };

    TEST_F(ProfileTest, ReadValueInt_Default)
    {
        int result = 0;
        EXPECT_TRUE(m_pProfile->ReadValue(L"Settings", L"SomeIntegerKey", result, 42));
        EXPECT_EQ(result, 42);
    }

    TEST_F(ProfileTest, ReadValueInt_Positive)
    {
        int result = 0;
        EXPECT_TRUE(m_pProfile->WriteValue(L"Settings", L"SomeIntegerKey", 100));
        EXPECT_TRUE(m_pProfile->ReadValue(L"Settings", L"SomeIntegerKey", result));
        EXPECT_EQ(result, 100);
    }

    TEST_F(ProfileTest, ReadValueInt_Missing)
    {
        int result = 0;
        EXPECT_TRUE(m_pProfile->ReadValue(L"Settings", L"SomeIntegerKey", result));
        EXPECT_EQ(result, 0);
    }

    TEST_F(ProfileTest, ReadValueString_Default)
    {
        std::wstring result;
        EXPECT_TRUE(m_pProfile->ReadValue(L"Settings", L"SomeStringKey", result, L"default"));
        EXPECT_EQ(result, L"default");
    }

    TEST_F(ProfileTest, ReadValueString_Positive)
    {
        std::wstring result;
        EXPECT_TRUE(m_pProfile->WriteValue(L"Settings", L"SomeStringKey", L"It will be written there."));
        EXPECT_TRUE(m_pProfile->ReadValue(L"Settings", L"SomeStringKey", result));
        EXPECT_EQ(result, L"It will be written there.");
    }

    TEST_F(ProfileTest, ReadValueString_Missing)
    {
        std::wstring result;
        EXPECT_TRUE(m_pProfile->ReadValue(L"Settings", L"SomeStringKey", result));
        EXPECT_EQ(result, L"");
    }

    TEST_F(ProfileTest, GetSettings_Defualt)
    {
        Setting setting {};
        EXPECT_TRUE(m_pProfile->GetSettings(setting));

        EXPECT_EQ(setting.lineEnding, LineEnding::AUTO);

        EXPECT_EQ(setting.lineFormat, LineFormat::DEFAULT);

        EXPECT_EQ(setting.indent.len, 4u);
        EXPECT_EQ(setting.indent.style, IndentStyle::AUTO);

        EXPECT_EQ(setting.bFollowCurrentTab, false);
        EXPECT_EQ(setting.bAutoFormat, false);
        EXPECT_EQ(setting.bUseJsonHighlight, true);

        EXPECT_EQ(setting.parseOptions.bIgnoreComment, true);
        EXPECT_EQ(setting.parseOptions.bIgnoreTrailingComma, true);
        EXPECT_EQ(setting.parseOptions.bReplaceUndefined, false);
    }

    TEST_F(ProfileTest, SetSettings_Positive)
    {
        Setting expected, actual;
        expected.lineEnding = LineEnding::WINDOWS;
        expected.lineFormat, LineFormat::SINGLELINE;
        expected.indent.len   = 2u;
        expected.indent.style = IndentStyle::TAB;

        expected.bAutoFormat       = true;
        expected.bFollowCurrentTab = true;
        expected.bAutoFormat       = true;
        expected.bUseJsonHighlight = false;

        expected.parseOptions.bIgnoreComment       = false;
        expected.parseOptions.bIgnoreTrailingComma = false;
        expected.parseOptions.bReplaceUndefined    = true;

        EXPECT_TRUE(m_pProfile->SetSettings(expected));
        EXPECT_TRUE(m_pProfile->GetSettings(actual));

        EXPECT_EQ(actual.lineEnding, expected.lineEnding);

        EXPECT_EQ(actual.lineFormat, expected.lineFormat);

        EXPECT_EQ(actual.indent.len, expected.indent.len);
        EXPECT_EQ(actual.indent.style, expected.indent.style);

        EXPECT_EQ(actual.bFollowCurrentTab, expected.bFollowCurrentTab);
        EXPECT_EQ(actual.bAutoFormat, expected.bAutoFormat);
        EXPECT_EQ(actual.bUseJsonHighlight, expected.bUseJsonHighlight);

        EXPECT_EQ(actual.parseOptions.bIgnoreComment, expected.parseOptions.bIgnoreComment);
        EXPECT_EQ(actual.parseOptions.bIgnoreTrailingComma, expected.parseOptions.bIgnoreTrailingComma);
        EXPECT_EQ(actual.parseOptions.bReplaceUndefined, expected.parseOptions.bReplaceUndefined);
    }
}    // namespace ProfileSettingTests
