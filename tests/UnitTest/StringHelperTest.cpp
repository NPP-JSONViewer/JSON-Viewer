#include <gtest/gtest.h>

#include <vector>
#include <string>

#include "StringHelper.h"

namespace Utility
{
    class StringHelperTest : public ::testing::Test
    {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

    // Test ReplaceAll with simple strings
    TEST_F(StringHelperTest, ReplaceAll_ShouldReplaceSubstring)
    {
        std::string result = StringHelper::ReplaceAll("hello world", "world", "C++");
        EXPECT_EQ(result, "hello C++");
    }

    TEST_F(StringHelperTest, ReplaceAll_ShouldReplaceMultipleOccurrences)
    {
        std::string result = StringHelper::ReplaceAll("a quick brown fox jumps over the lazy dog", "o", "0");
        EXPECT_EQ(result, "a quick br0wn f0x jumps 0ver the lazy d0g");
    }

    TEST_F(StringHelperTest, ReplaceAll_NoOccurrences)
    {
        std::string result = StringHelper::ReplaceAll("hello world", "foo", "bar");
        EXPECT_EQ(result, "hello world");
    }

    // Test ReplaceAll for wide strings
    TEST_F(StringHelperTest, ReplaceAll_WideString_ShouldReplaceSubstring)
    {
        std::wstring result = StringHelper::ReplaceAll(L"hello world", L"world", L"C++");
        EXPECT_EQ(result, L"hello C++");
    }

    TEST_F(StringHelperTest, ReplaceAll_WideString_NoOccurrences)
    {
        std::wstring result = StringHelper::ReplaceAll(L"hello world", L"foo", L"bar");
        EXPECT_EQ(result, L"hello world");
    }

    // Test ToWstring with various encodings
    TEST_F(StringHelperTest, ToWstring_ShouldConvertString)
    {
        std::wstring result = StringHelper::ToWstring("hello", CP_UTF8);
        EXPECT_EQ(result, L"hello");
    }

    TEST_F(StringHelperTest, ToWstring_ShouldHandleEmptyString)
    {
        std::wstring result = StringHelper::ToWstring("", CP_UTF8);
        EXPECT_EQ(result, L"");
    }

    TEST_F(StringHelperTest, ToWstring_InvalidEncoding_ShouldReturnEmpty)
    {
        std::wstring result = StringHelper::ToWstring("invalid", 9999);    // Invalid codepage
        EXPECT_EQ(result, L"");
    }

    // Test ToString with various encodings
    TEST_F(StringHelperTest, ToString_ShouldConvertWstring)
    {
        std::string result = StringHelper::ToString(L"hello", CP_UTF8);
        EXPECT_EQ(result, "hello");
    }

    TEST_F(StringHelperTest, ToString_ShouldHandleEmptyWstring)
    {
        std::string result = StringHelper::ToString(L"", CP_UTF8);
        EXPECT_EQ(result, "");
    }

    TEST_F(StringHelperTest, ToString_InvalidEncoding_ShouldReturnEmpty)
    {
        std::string result = StringHelper::ToString(L"invalid", 9999);    // Invalid codepage
        EXPECT_EQ(result, "");
    }

    // Test Split for standard strings
    TEST_F(StringHelperTest, Split_ShouldSplitStringByDelimiter)
    {
        std::vector<std::string> result   = StringHelper::Split("a,b,c", ",");
        std::vector<std::string> expected = {"a", "b", "c"};
        EXPECT_EQ(result.size(), expected.size());
        EXPECT_EQ(result, expected);
    }

    TEST_F(StringHelperTest, Split_ShouldHandleEmptyString)
    {
        std::vector<std::string> result = StringHelper::Split("", ",");
        EXPECT_TRUE(result.empty());
    }

    TEST_F(StringHelperTest, Split_ShouldHandleNoDelimiters)
    {
        std::vector<std::string> result   = StringHelper::Split("abc", ",");
        std::vector<std::string> expected = {"abc"};
        EXPECT_EQ(result.size(), expected.size());
        EXPECT_EQ(result, expected);
    }

    // Test Split for wide strings
    TEST_F(StringHelperTest, Split_WideString_ShouldSplitStringByDelimiter)
    {
        std::vector<std::wstring> result   = StringHelper::Split(L"a,b,c", L",");
        std::vector<std::wstring> expected = {L"a", L"b", L"c"};
        EXPECT_EQ(result.size(), expected.size());
        EXPECT_EQ(result, expected);
    }

    TEST_F(StringHelperTest, Split_WideString_ShouldHandleEmptyString)
    {
        std::vector<std::wstring> result = StringHelper::Split(L"", L",");
        EXPECT_TRUE(result.empty());
    }

    TEST_F(StringHelperTest, Split_WideString_ShouldHandleNoDelimiters)
    {
        std::vector<std::wstring> result   = StringHelper::Split(L"abc", L",");
        std::vector<std::wstring> expected = {L"abc"};
        EXPECT_EQ(result.size(), expected.size());
        EXPECT_EQ(result, expected);
    }

    // Test Contains method with case sensitivity
    TEST_F(StringHelperTest, Contains_ShouldFindSubstring)
    {
        bool result = StringHelper::Contains("hello world", "world", false);
        EXPECT_TRUE(result);
    }

    TEST_F(StringHelperTest, Contains_ShouldReturnFalseForNonExistingSubstring)
    {
        bool result = StringHelper::Contains("hello world", "foo", false);
        EXPECT_FALSE(result);
    }

    TEST_F(StringHelperTest, Contains_IgnoreCase_ShouldFindSubstring)
    {
        bool result = StringHelper::Contains("Hello World", "world", true);
        EXPECT_TRUE(result);
    }

    // Test ToLower
    TEST_F(StringHelperTest, ToLower_ShouldConvertStringToLowercase)
    {
        std::string input = "HeLLo WoRLD";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, "hello world");
    }

    TEST_F(StringHelperTest, ToLower_WideString_ShouldConvertToLowercase)
    {
        std::wstring input = L"HeLLo WoRLD";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, L"hello world");
    }
}    // namespace Utility
