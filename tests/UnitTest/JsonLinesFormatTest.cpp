#include <gtest/gtest.h>
#include "JsonHandler.h"

TEST(JsonLinesFormatTest, FormatsRecordsUsingExistingSettings)
{
    JsonHandler handler({});
    const auto result = handler.FormatJsonLines("{\"a\":1}\n{\"b\":[2,3]}", LE::kLf, LF::kFormatSingleLineArray, '\t', 1);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "{\n\t\"a\": 1\n}\n{\n\t\"b\": [2, 3]\n}");
}

TEST(JsonLinesFormatTest, HandlesLineEndingsAndFinalNewline)
{
    JsonHandler handler({});
    for (const auto& inputEol : {"\n", "\r\n", "\r"})
    {
        const std::string input = std::string("{}") + inputEol + "[]" + inputEol;
        for (const auto& [ending, expectedEol] : {std::pair{LE::kLf, "\n"}, std::pair{LE::kCrLf, "\r\n"}, std::pair{LE::kCr, "\r"}})
        {
            const auto result = handler.FormatJsonLines(input, ending, {}, ' ', 2);
            ASSERT_TRUE(result.has_value());
            EXPECT_EQ(*result, std::string("{}") + expectedEol + "[]" + expectedEol);
        }
    }
}

TEST(JsonLinesFormatTest, SkipsBlankLinesAndPreservesNumericText)
{
    JsonHandler handler({});
    const auto result = handler.FormatJsonLines("\n \t\r\n{\"n\":99999999999999999999}\n\n1e+10\n", LE::kLf, {}, ' ', 2);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "{\n  \"n\": 99999999999999999999\n}\n1e+10\n");
}

TEST(JsonLinesFormatTest, SupportsAllRootValueTypes)
{
    JsonHandler handler({});
    const auto result = handler.FormatJsonLines("{}\n[]\n\"text\\nvalue\"\n42\ntrue\nfalse\nnull", LE::kLf, {}, ' ', 2);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "{}\n[]\n\"text\\nvalue\"\n42\ntrue\nfalse\nnull");
}

TEST(JsonLinesFormatTest, ReportsOriginalErrorOffsetWithoutPartialOutput)
{
    JsonHandler handler({});
    const std::string badLine = "{\"b\":}";
    const std::string prefix = "{}\r\n\r\n";
    const auto single = handler.FormatJson(badLine, LE::kLf, {}, ' ', 2);
    const auto result = handler.FormatJsonLines(prefix + badLine + "\r\n{}", LE::kLf, {}, ' ', 2);
    ASSERT_FALSE(result.has_value());

    EXPECT_EQ(result.error().position, static_cast<int>(prefix.size()) + single.error().position);
    EXPECT_EQ(result.error().code, single.error().code);
    EXPECT_EQ(result.error().message, "Line 3: " + single.error().message);
}

TEST(JsonLinesFormatTest, RejectsMultipleValuesOnOneLineAndMultilineRecords)
{
    JsonHandler handler({});
    EXPECT_FALSE(handler.FormatJsonLines("{} {}", LE::kLf, {}, ' ', 2).has_value());
    EXPECT_FALSE(handler.FormatJsonLines("{\n\"a\":1\n}", LE::kLf, {}, ' ', 2).has_value());
    EXPECT_FALSE(handler.FormatJsonLines(" \t\r\n", LE::kLf, {}, ' ', 2).has_value());
    EXPECT_FALSE(handler.FormatJsonLines("", LE::kLf, {}, ' ', 2).has_value());
}

TEST(JsonLinesFormatTest, HonorsRelaxedParsingSettings)
{
    ParseOptions options;
    JsonHandler relaxed(options);
    const std::string input = "{\"a\":1,}\n{/* comment */\"b\":2}";
    EXPECT_TRUE(relaxed.FormatJsonLines(input, LE::kLf, {}, ' ', 2).has_value());
    options.bIgnoreTrailingComma = false;
    EXPECT_FALSE(JsonHandler(options).FormatJsonLines(input, LE::kLf, {}, ' ', 2).has_value());
    options.bIgnoreTrailingComma = true;
    options.bIgnoreComment = false;
    EXPECT_FALSE(JsonHandler(options).FormatJsonLines(input, LE::kLf, {}, ' ', 2).has_value());
}
