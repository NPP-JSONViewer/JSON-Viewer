#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include "JsonHandler.h"

namespace JsonFormat
{
    class JsonFormatTest : public ::testing::Test
    {
    protected:
        JsonHandler m_jsonFormatter {{}};

    protected:
        void SetUp() override {}
        void TearDown() override {}

        void setParseOptions(const ParseOptions &opt)
        {
            m_jsonFormatter = JsonHandler(opt);
        }
    };

    // Test 1: Test valid JSON with default formatting options
    TEST_F(JsonFormatTest, FormatJson_ValidJson_DefaultOptions)
    {
        std::string inputJson        = R"({"name": "John", "age": 30})";
        LE          lineEndingOption = rj::kLf;
        LF          formatOptions    = rj::kFormatDefault;
        char        indentChar       = ' ';
        unsigned    indentLen        = 4;

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, "{\n    \"name\": \"John\",\n    \"age\": 30\n}");
    }

    // Test 2: Test invalid JSON input
    TEST_F(JsonFormatTest, FormatJson_InvalidJson)
    {
        std::string inputJson        = R"({"name": "John", "age": })";    // Invalid JSON (missing age value)
        LE          lineEndingOption = rj::kLf;
        LF          formatOptions    = rj::kFormatDefault;
        char        indentChar       = ' ';
        unsigned    indentLen        = 4;

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_FALSE(result.success);
        EXPECT_NE(result.error_code, -1);
        EXPECT_NE(result.error_pos, -1);
        EXPECT_FALSE(result.error_str.empty());
        EXPECT_EQ(result.response, "");
    }

    // Test 3: Test valid JSON with custom indentation (tabs instead of spaces)
    TEST_F(JsonFormatTest, FormatJson_ValidJson_TabIndentation)
    {
        std::string inputJson        = R"({"name": "John", "age": 30})";
        LE          lineEndingOption = rj::kLf;
        LF          formatOptions    = rj::kFormatDefault;    // Default
        char        indentChar       = '\t';                  // Tab instead of space
        unsigned    indentLen        = 1;                     // Single tab indentation

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, "{\n\t\"name\": \"John\",\n\t\"age\": 30\n}");
    }

    // Test 4: Test valid JSON with Windows line endings
    TEST_F(JsonFormatTest, FormatJson_ValidJson_WindowsLineEndings)
    {
        std::string inputJson        = R"({"name": "John", "age": 30})";
        LE          lineEndingOption = rj::kCrLf;             // Windows line ending
        LF          formatOptions    = rj::kFormatDefault;    // Default
        char        indentChar       = ' ';
        unsigned    indentLen        = 4;

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, "{\r\n    \"name\": \"John\",\r\n    \"age\": 30\r\n}");
    }

    // Test 5: Test valid JSON with no indentation (compact format)
    TEST_F(JsonFormatTest, FormatJson_ValidJson_NoIndentation)
    {
        std::string inputJson        = R"({"name": "John", "age": 30})";
        LE          lineEndingOption = rj::kCrLf;                     // Windows line ending
        LF          formatOptions    = rj::kFormatSingleLineArray;    // Custom option for no pretty formatting
        char        indentChar       = ' ';
        unsigned    indentLen        = 0;    // No indentation

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, "{\r\n\"name\": \"John\",\r\n\"age\": 30\r\n}");
    }

    // Test 6: Test JSON with comments ignored
    TEST_F(JsonFormatTest, FormatJson_IgnoreComments)
    {
        std::string inputJson        = R"({
        // Comment here
        "name": "John", 
        "age": 30
    })";
        LE          lineEndingOption = rj::kCrLf;             // Windows line ending
        LF          formatOptions    = rj::kFormatDefault;    // Default
        char        indentChar       = ' ';
        unsigned    indentLen        = 4;

        // Set parse options to ignore comments
        ParseOptions parseOptions {.bIgnoreComment = true, .bIgnoreTrailingComma = false, .bReplaceUndefined = false};
        setParseOptions(parseOptions);

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, "{\r\n    \"name\": \"John\",\r\n    \"age\": 30\r\n}");
    }

    // Test 7: Test JSON with trailing commas ignored
    TEST_F(JsonFormatTest, FormatJson_IgnoreTrailingCommas)
    {
        std::string inputJson        = R"({
        "name": "John",
        "age": 30,
    })";                                               // Trailing comma is invalid in standard JSON
        LE          lineEndingOption = rj::kCrLf;             // Windows line ending
        LF          formatOptions    = rj::kFormatDefault;    // Default
        char        indentChar       = ' ';
        unsigned    indentLen        = 2;

        // Set parse options to ignore trailing commas
        ParseOptions parseOptions {.bIgnoreComment = false, .bIgnoreTrailingComma = true, .bReplaceUndefined = false};
        setParseOptions(parseOptions);

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, "{\r\n  \"name\": \"John\",\r\n  \"age\": 30\r\n}");
    }

    // Test 8: Test with infinity NaN inf
    TEST_F(JsonFormatTest, FormatJson_Infinity_NaN_Null)
    {
        std::unordered_map<std::string, std::string> testData {
            {R"({"NaN":NaN})", "{\r\n    \"NaN\": NaN\r\n}"},
            {R"({"Mixed":[null,null,"power"]})", "{\r\n    \"Mixed\": [\r\n        null,\r\n        null,\r\n        \"power\"\r\n    ]\r\n}"},
            {R"({"Inf":[-Infinity, Infinity, -Inf, Inf]})",
             "{\r\n    \"Inf\": [\r\n        -Infinity,\r\n        Infinity,\r\n        -Infinity,\r\n        Infinity\r\n    ]\r\n}"},
        };

        LE       lineEndingOption = rj::kCrLf;             // Windows line ending
        LF       formatOptions    = rj::kFormatDefault;    // Custom option for no pretty formatting
        char     indentChar       = ' ';
        unsigned indentLen        = 4;    // No indentation

        for (const auto &[input, output] : testData)
        {
            auto result = m_jsonFormatter.FormatJson(input, lineEndingOption, formatOptions, indentChar, indentLen);

            EXPECT_TRUE(result.success);
            EXPECT_EQ(result.error_code, -1);
            EXPECT_EQ(result.error_pos, -1);
            EXPECT_EQ(result.error_str, "");
            EXPECT_EQ(result.response, output);
        }
    }

    // Test 9: Test with infinity NaN inf , single line on array
    TEST_F(JsonFormatTest, FormatJson_Infinity_NaN_Null_SingleLineOnArray)
    {
        std::unordered_map<std::string, std::string> testData {
            {R"({"NaN":NaN})", "{\r\n    \"NaN\": NaN\r\n}"},
            {R"({"Mixed":[null,null,"power"]})", "{\r\n    \"Mixed\": [null, null, \"power\"]\r\n}"},
            {R"({"Inf":[-Infinity, Infinity, -Inf, Inf]})", "{\r\n    \"Inf\": [-Infinity, Infinity, -Infinity, Infinity]\r\n}"},
        };

        LE       lineEndingOption = rj::kCrLf;                     // Windows line ending
        LF       formatOptions    = rj::kFormatSingleLineArray;    // Custom option for no pretty formatting
        char     indentChar       = ' ';
        unsigned indentLen        = 4;    // No indentation

        for (const auto &[input, output] : testData)
        {
            auto result = m_jsonFormatter.FormatJson(input, lineEndingOption, formatOptions, indentChar, indentLen);

            EXPECT_TRUE(result.success);
            EXPECT_EQ(result.error_code, -1);
            EXPECT_EQ(result.error_pos, -1);
            EXPECT_EQ(result.error_str, "");
            EXPECT_EQ(result.response, output);
        }
    }

    // Test 10: Test valid JSON with unicode sequence
    TEST_F(JsonFormatTest, FormatJson_UnicodeSequence)
    {
        std::string inputJson        = R"({"FreeTextInput":"\u003Cscript\u003Ealert(\u0022links\u0022);\u003C/script\u003E"})";
        LE          lineEndingOption = rj::kCrLf;             // Windows line ending
        LF          formatOptions    = rj::kFormatDefault;    // Custom option for no pretty formatting
        char        indentChar       = ' ';                   // space indented
        unsigned    indentLen        = 0;                     // No indentation

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, "{\r\n\"FreeTextInput\": \"<script>alert(\\\"links\\\");</script>\"\r\n}");
    }

    // Test 11: Test with numbers (with and without precision)
    TEST_F(JsonFormatTest, FormatJson_numbers)
    {
        LE       lineEndingOption = rj::kCrLf;             // Windows line ending
        LF       formatOptions    = rj::kFormatDefault;    // Custom option for no pretty formatting
        char     indentChar       = ' ';                   // space indented
        unsigned indentLen        = 4;                     // No indentation

        std::unordered_map<std::string, std::string> testData {
            {R"({"num": 12.148681171238422})", "12.148681171238422"},             // All good
            {R"({"num": 42.835353759876654})", "42.835353759876654"},             // All good
            {R"({"num": 5.107091491635510056019771245})", "5.10709149163551"},    // Fine: Rounded up
            {R"({"num": 100000000302052988.0})", "100000000302052990.0"},         // Fine: Rounded up
            {R"({"num": 12.148681171238427111})", "12.148681171238428"},          // Fine: Rounded down
            {R"({"num": 42.8353537598766541666})", "42.835353759876654"},         // Fine: Rounded up
            {R"({"num": -1722.1864265316147})", "-1722.1864265316146"},           // This is interesting. Why last digit changed.
            {R"({"num": -1722.1864265316148})", "-1722.1864265316149"},           // This is interesting. Why last digit changed.
            {R"({"num": -172345.18642653167979})", "-172345.18642653167"},        // This is interesting. Why not rounded up.
            {R"({"num": 1.234e5})", "123400.0"},                                  // Don't know how to fix.
            {R"({"num": 0.0000001000})", "1e-7"},                                 // Don't know how to fix.
        };

        for (const auto &[input, output] : testData)
        {
            auto result      = m_jsonFormatter.FormatJson(input, lineEndingOption, formatOptions, indentChar, indentLen);
            bool foundNumber = result.response.find(output) != std::string::npos;
            EXPECT_TRUE(result.success);
            EXPECT_EQ(result.error_code, -1);
            EXPECT_EQ(result.error_pos, -1);
            EXPECT_EQ(result.error_str, "");
            EXPECT_EQ(foundNumber, true) << "Number: '" << output.c_str() << "' not found in '" << result.response.c_str() << "'.\n";
        }
    }

    // Test 12: Test with different type of data
    TEST_F(JsonFormatTest, FormatJson_MultipleDataType)
    {
        const std::string inputJson  = R"({
	"name": "npp-pluginList",
	"version": "1.5.3",
	"List": {
		"defaultTimeInterval": 123400.0
	},
	"Array": [
		{
			"Text": "I am text",
			"Bool": true,
			"Number": 123456
		},
		{
			"Text": "",
			"Bool": false,
			"Number": 0,
			"null": null
		}
	]
})";
        const std::string outputJson = "{\r\n  \"name\": \"npp-pluginList\",\r\n  \"version\": \"1.5.3\",\r\n  \"List\": {\r\n    \"defaultTimeInterval\": 123400.0\r\n  },\r\n  "
                                       "\"Array\": [\r\n    {\r\n      \"Text\": \"I am text\",\r\n      \"Bool\": true,\r\n      \"Number\": 123456\r\n    },\r\n    {\r\n      "
                                       "\"Text\": \"\",\r\n      \"Bool\": false,\r\n      \"Number\": 0,\r\n      \"null\": null\r\n    }\r\n  ]\r\n}";

        LE       lineEndingOption = rj::kCrLf;             // Windows line ending
        LF       formatOptions    = rj::kFormatDefault;    // Default
        char     indentChar       = ' ';
        unsigned indentLen        = 2;

        // Set parse options to ignore trailing commas
        ParseOptions parseOptions {.bIgnoreComment = false, .bIgnoreTrailingComma = true, .bReplaceUndefined = false};
        setParseOptions(parseOptions);

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, outputJson);
    }

    // Test 13: Test with different type of data in array on single line
    TEST_F(JsonFormatTest, FormatJson_MultipleDataType_SingleLine)
    {
        const std::string inputJson  = R"({
	"name": "npp-pluginList",
	"version": "1.5.3",
	"List": {
		"defaultTimeInterval": 123400.0
	},
	"Array": [
		{
			"Text": "I am text",
			"Bool": true,
			"Number": 123456
		},
		{
			"Text": "",
			"Bool": false,
			"Number": 0,
			"null": null
		}
	]
})";
        const std::string outputJson = "{\r\n  \"name\": \"npp-pluginList\",\r\n  \"version\": \"1.5.3\",\r\n  \"List\": {\r\n    \"defaultTimeInterval\": 123400.0\r\n  },\r\n  "
                                       "\"Array\": [{\r\n      \"Text\": \"I am text\",\r\n      \"Bool\": true,\r\n      \"Number\": 123456\r\n    }, {\r\n      \"Text\": "
                                       "\"\",\r\n      \"Bool\": false,\r\n      \"Number\": 0,\r\n      \"null\": null\r\n    }]\r\n}";


        LE       lineEndingOption = rj::kCrLf;                     // Windows line ending
        LF       formatOptions    = rj::kFormatSingleLineArray;    // Default
        char     indentChar       = ' ';
        unsigned indentLen        = 2;

        // Set parse options to ignore trailing commas
        ParseOptions parseOptions {.bIgnoreComment = false, .bIgnoreTrailingComma = true, .bReplaceUndefined = false};
        setParseOptions(parseOptions);

        auto result = m_jsonFormatter.FormatJson(inputJson, lineEndingOption, formatOptions, indentChar, indentLen);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, outputJson);
    }

}    // namespace JsonFormat
