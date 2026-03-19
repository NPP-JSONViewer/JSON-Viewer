#include <gtest/gtest.h>

#include <vector>
#include <string>

#include "StringHelper.h"

namespace UtilityTest
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

    // ==================== ReplaceAll Tests ====================
    class StringHelperReplaceAllTest : public ::testing::Test
    {
    };

    TEST_F(StringHelperReplaceAllTest, ReplaceAll_SimpleString)
    {
        std::string input  = "Hello World Hello";
        std::string result = StringHelper::ReplaceAll(input, "Hello", "Hi");
        EXPECT_EQ(result, "Hi World Hi");
    }

    TEST_F(StringHelperReplaceAllTest, ReplaceAll_EmptySearch)
    {
        std::string input  = "Hello World";
        std::string result = StringHelper::ReplaceAll(input, "", "X");
        EXPECT_EQ(result, "Hello World");
    }

    TEST_F(StringHelperReplaceAllTest, ReplaceAll_EmptyInput)
    {
        std::string input  = "";
        std::string result = StringHelper::ReplaceAll(input, "test", "X");
        EXPECT_EQ(result, "");
    }

    TEST_F(StringHelperReplaceAllTest, ReplaceAll_NoMatch)
    {
        std::string input  = "Hello World";
        std::string result = StringHelper::ReplaceAll(input, "xyz", "abc");
        EXPECT_EQ(result, "Hello World");
    }

    TEST_F(StringHelperReplaceAllTest, ReplaceAll_RegexPattern)
    {
        std::string input  = "test123abc456def";
        std::string result = StringHelper::ReplaceAll(input, "[0-9]+", "X");
        EXPECT_EQ(result, "testXabcXdef");
    }

    TEST_F(StringHelperReplaceAllTest, ReplaceAll_InvalidRegex_FallbackToLiteral)
    {
        // Invalid regex should fallback to literal replacement
        std::string input  = "test[invalid";
        std::string result = StringHelper::ReplaceAll(input, "[invalid", "fixed");
        EXPECT_EQ(result, "testfixed");
    }

    TEST_F(StringHelperReplaceAllTest, ReplaceAll_Wide)
    {
        std::wstring input  = L"Hello World Hello";
        std::wstring result = StringHelper::ReplaceAll(input, L"Hello", L"Hi");
        EXPECT_EQ(result, L"Hi World Hi");
    }

    // ==================== ReplaceLiteral Tests ====================
    class StringHelperReplaceLiteralTest : public ::testing::Test
    {
    };

    TEST_F(StringHelperReplaceLiteralTest, ReplaceLiteral_SimpleString)
    {
        std::string input  = "Hello World Hello";
        std::string result = StringHelper::ReplaceLiteral(input, "Hello", "Hi");
        EXPECT_EQ(result, "Hi World Hi");
    }

    TEST_F(StringHelperReplaceLiteralTest, ReplaceLiteral_RegexCharacters)
    {
        // Test that regex special characters are treated literally
        std::string input  = "test.+.*test";
        std::string result = StringHelper::ReplaceLiteral(input, ".+.*", "X");
        EXPECT_EQ(result, "testXtest");
    }

    TEST_F(StringHelperReplaceLiteralTest, ReplaceLiteral_OverlappingPatterns)
    {
        std::string input  = "aaaa";
        std::string result = StringHelper::ReplaceLiteral(input, "aa", "b");
        EXPECT_EQ(result, "bb");
    }

    TEST_F(StringHelperReplaceLiteralTest, ReplaceLiteral_EmptySearch)
    {
        std::string input  = "Hello";
        std::string result = StringHelper::ReplaceLiteral(input, "", "X");
        EXPECT_EQ(result, "Hello");
    }

    TEST_F(StringHelperReplaceLiteralTest, ReplaceLiteral_Wide)
    {
        std::wstring input  = L"test.+.* pattern";
        std::wstring result = StringHelper::ReplaceLiteral(input, L".+.*", L"X");
        EXPECT_EQ(result, L"testX pattern");
    }

    // ==================== ToWstring/ToString Tests ====================
    class StringHelperConversionTest : public ::testing::Test
    {
    };

    TEST_F(StringHelperConversionTest, ToWstring_ValidAscii)
    {
        std::string  input  = "Hello World";
        std::wstring result = StringHelper::ToWstring(input);
        EXPECT_EQ(result, L"Hello World");
    }

    TEST_F(StringHelperConversionTest, ToWstring_EmptyString)
    {
        std::string  input  = "";
        std::wstring result = StringHelper::ToWstring(input);
        EXPECT_EQ(result, L"");
    }

    TEST_F(StringHelperConversionTest, ToWstring_SpecialCharacters)
    {
        std::string  input  = "!@#$%^&*()";
        std::wstring result = StringHelper::ToWstring(input);
        EXPECT_EQ(result, L"!@#$%^&*()");
    }

    TEST_F(StringHelperConversionTest, ToString_ValidWide)
    {
        std::wstring input  = L"Hello World";
        std::string  result = StringHelper::ToString(input);
        EXPECT_EQ(result, "Hello World");
    }

    TEST_F(StringHelperConversionTest, ToString_EmptyString)
    {
        std::wstring input  = L"";
        std::string  result = StringHelper::ToString(input);
        EXPECT_EQ(result, "");
    }

    TEST_F(StringHelperConversionTest, RoundTrip_StringToWstringToString)
    {
        std::string  original = "Test String 123";
        std::wstring wide     = StringHelper::ToWstring(original);
        std::string  result   = StringHelper::ToString(wide);
        EXPECT_EQ(result, original);
    }

    // ==================== Split Tests ====================
    class StringHelperSplitTest : public ::testing::Test
    {
    };

    TEST_F(StringHelperSplitTest, Split_SingleDelimiter)
    {
        std::string input  = "a,b,c,d";
        auto        result = StringHelper::Split(input, ",");
        EXPECT_EQ(result.size(), 4);
        EXPECT_EQ(result[0], "a");
        EXPECT_EQ(result[1], "b");
        EXPECT_EQ(result[2], "c");
        EXPECT_EQ(result[3], "d");
    }

    TEST_F(StringHelperSplitTest, Split_ConsecutiveDelimiters)
    {
        std::string input  = "a,,b,,c";
        auto        result = StringHelper::Split(input, ",");
        // Consecutive delimiters should be treated as one
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result[0], "a");
        EXPECT_EQ(result[1], "b");
        EXPECT_EQ(result[2], "c");
    }

    TEST_F(StringHelperSplitTest, Split_DelimiterAtStart)
    {
        std::string input  = ",a,b,c";
        auto        result = StringHelper::Split(input, ",");
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result[0], "a");
        EXPECT_EQ(result[1], "b");
        EXPECT_EQ(result[2], "c");
    }

    TEST_F(StringHelperSplitTest, Split_DelimiterAtEnd)
    {
        std::string input  = "a,b,c,";
        auto        result = StringHelper::Split(input, ",");
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result[0], "a");
        EXPECT_EQ(result[1], "b");
        EXPECT_EQ(result[2], "c");
    }

    TEST_F(StringHelperSplitTest, Split_EmptyInput)
    {
        std::string input  = "";
        auto        result = StringHelper::Split(input, ",");
        EXPECT_EQ(result.size(), 0);
    }

    TEST_F(StringHelperSplitTest, Split_EmptyDelimiter)
    {
        std::string input  = "abc";
        auto        result = StringHelper::Split(input, "");
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], "abc");
    }

    TEST_F(StringHelperSplitTest, Split_MultiCharDelimiter)
    {
        std::string input  = "a::b::c::d";
        auto        result = StringHelper::Split(input, "::");
        EXPECT_EQ(result.size(), 4);
        EXPECT_EQ(result[0], "a");
        EXPECT_EQ(result[3], "d");
    }

    TEST_F(StringHelperSplitTest, Split_NoDelimitersFound)
    {
        std::string input  = "abcdef";
        auto        result = StringHelper::Split(input, ",");
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], "abcdef");
    }

    TEST_F(StringHelperSplitTest, Split_Wide)
    {
        std::wstring input  = L"a;b;c;d";
        auto         result = StringHelper::Split(input, L";");
        EXPECT_EQ(result.size(), 4);
        EXPECT_EQ(result[0], L"a");
        EXPECT_EQ(result[3], L"d");
    }

    // ==================== Contains Tests ====================
    class StringHelperContainsTest : public ::testing::Test
    {
    };

    TEST_F(StringHelperContainsTest, Contains_StringFound)
    {
        std::string input  = "Hello World";
        bool        result = StringHelper::Contains(input, "World");
        EXPECT_TRUE(result);
    }

    TEST_F(StringHelperContainsTest, Contains_StringNotFound)
    {
        std::string input  = "Hello World";
        bool        result = StringHelper::Contains(input, "xyz");
        EXPECT_FALSE(result);
    }

    TEST_F(StringHelperContainsTest, Contains_CaseInsensitive)
    {
        std::string input  = "Hello World";
        bool        result = StringHelper::Contains(input, "hello", true);
        EXPECT_TRUE(result);
    }

    TEST_F(StringHelperContainsTest, Contains_CaseSensitive)
    {
        std::string input  = "Hello World";
        bool        result = StringHelper::Contains(input, "hello", false);
        EXPECT_FALSE(result);
    }

    TEST_F(StringHelperContainsTest, Contains_EmptySearch)
    {
        std::string input  = "Hello World";
        bool        result = StringHelper::Contains(input, "");
        EXPECT_TRUE(result);    // Empty string is found in any string
    }

    TEST_F(StringHelperContainsTest, Contains_EmptyInput)
    {
        std::string input  = "";
        bool        result = StringHelper::Contains(input, "test");
        EXPECT_FALSE(result);
    }

    TEST_F(StringHelperContainsTest, Contains_Wide)
    {
        std::wstring input  = L"Hello World";
        bool         result = StringHelper::Contains(input, L"World");
        EXPECT_TRUE(result);
    }

    TEST_F(StringHelperContainsTest, Contains_SpecialCharacters)
    {
        std::string input  = "test@example.com";
        bool        result = StringHelper::Contains(input, "@example");
        EXPECT_TRUE(result);
    }

    // ==================== ToLower Tests ====================
    class StringHelperToLowerTest : public ::testing::Test
    {
    };

    TEST_F(StringHelperToLowerTest, ToLower_UppercaseString)
    {
        std::string input = "HELLO WORLD";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, "hello world");
    }

    TEST_F(StringHelperToLowerTest, ToLower_MixedCase)
    {
        std::string input = "HeLLo WoRLD";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, "hello world");
    }

    TEST_F(StringHelperToLowerTest, ToLower_LowercaseString)
    {
        std::string input = "hello world";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, "hello world");
    }

    TEST_F(StringHelperToLowerTest, ToLower_WithNumbers)
    {
        std::string input = "Test123ABC";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, "test123abc");
    }

    TEST_F(StringHelperToLowerTest, ToLower_WithSpecialCharacters)
    {
        std::string input = "TEST!@#$%^&*()";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, "test!@#$%^&*()");
    }

    TEST_F(StringHelperToLowerTest, ToLower_EmptyString)
    {
        std::string input = "";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, "");
    }

    TEST_F(StringHelperToLowerTest, ToLower_Wide)
    {
        std::wstring input = L"HELLO WORLD";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, L"hello world");
    }

    TEST_F(StringHelperToLowerTest, ToLower_WideWithNumbers)
    {
        std::wstring input = L"Test123ABC";
        StringHelper::ToLower(input);
        EXPECT_EQ(input, L"test123abc");
    }
}    // namespace UtilityTest
