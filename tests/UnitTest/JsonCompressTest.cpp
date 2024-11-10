#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include "JsonHandler.h"

namespace JsonCompress
{
    class JsonCompressTest : public ::testing::Test
    {
    protected:
        JsonHandler m_jsonHandler {{}};

    protected:
        void SetUp() override {}
        void TearDown() override {}

        void setParseOptions(const ParseOptions& opt)
        {
            m_jsonHandler = JsonHandler(opt);
        }
    };

    // Test 1: Test valid JSON with default formatting options
    TEST_F(JsonCompressTest, CompressJson_ValidJson)
    {
        const std::string inputJson = R"({"name": "John", "age": 30})";

        auto result = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, R"({"name":"John","age":30})");
    }

    // Test 2: Test invalid JSON input
    TEST_F(JsonCompressTest, CompressJson_InvalidJson)
    {
        const std::string inputJson = R"({"name": "John", "age": })";    // Invalid JSON (missing age value)

        auto result = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_FALSE(result.success);
        EXPECT_NE(result.error_code, -1);
        EXPECT_NE(result.error_pos, -1);
        EXPECT_FALSE(result.error_str.empty());
        EXPECT_EQ(result.response, "");
    }

    // Test 3: Test valid JSON with tab indentation
    TEST_F(JsonCompressTest, CompressJson_ValidJson_TabIndentation)
    {
        const std::string inputJson = "{\n\t\"name\": \"John\",\n\t\"age\": 30\n}";

        auto result = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, R"({"name":"John","age":30})");
    }

    // Test 4: Test valid JSON with Windows line endings
    TEST_F(JsonCompressTest, CompressJson_ValidJson_WindowsLineEndings)
    {
        const std::string inputJson = "{\r\n    \"name\": \"John\",\r\n    \"age\": 30}";

        auto result = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, R"({"name":"John","age":30})");
    }

    // Test 5: Test valid JSON with no indentation (compact format)
    TEST_F(JsonCompressTest, CompressJson_ValidJson_NoIndentation)
    {
        std::string inputJson = R"({"name":"John","age":30})";
        auto        result    = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, R"({"name":"John","age":30})");
    }

    // Test 6: Test JSON with comments ignored
    TEST_F(JsonCompressTest, CompressJson_IgnoreComments)
    {
        const std::string inputJson = R"({
        // Comment here
        "name": "John", 
        "age": 30
    })";

        // Set parse options to ignore comments
        ParseOptions parseOptions {.bIgnoreComment = true, .bIgnoreTrailingComma = false, .bReplaceUndefined = false};
        setParseOptions(parseOptions);

        auto result = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, R"({"name":"John","age":30})");
    }

    // Test 7: Test JSON with trailing commas ignored
    TEST_F(JsonCompressTest, CompressJson_IgnoreTrailingCommas)
    {
        const std::string inputJson = R"({
        "name": "John",
        "age": 30,
    })";    // Trailing comma is invalid in standard JSON

        // Set parse options to ignore trailing commas
        ParseOptions parseOptions {.bIgnoreComment = false, .bIgnoreTrailingComma = true, .bReplaceUndefined = false};
        setParseOptions(parseOptions);

        auto result = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, R"({"name":"John","age":30})");
    }

    // Test 8: Test with infinity NaN inf
    TEST_F(JsonCompressTest, CompressJson_Infinity_NaN_Null)
    {
        std::unordered_map<std::string, std::string> testData {
            {"{\r\n    \"NaN\": NaN\r\n}", R"({"NaN":NaN})"},
            {"{\r\n    \"Mixed\": [\r\n        null,\r\n        null,\r\n        \"power\"\r\n    ]\r\n}", R"({"Mixed":[null,null,"power"]})"},
            {"{\n  \"Inf\": [\n    -Infinity,\n    Infinity,\n    -Inf,\n    Inf\n  ]\n}", R"({"Inf":[-Infinity,Infinity,-Infinity,Infinity]})"},
        };

        for (const auto& [input, output] : testData)
        {
            auto result = m_jsonHandler.GetCompressedJson(input);

            EXPECT_TRUE(result.success);
            EXPECT_EQ(result.error_code, -1);
            EXPECT_EQ(result.error_pos, -1);
            EXPECT_EQ(result.error_str, "");
            EXPECT_EQ(result.response, output);
        }
    }

    // Test 9: Test valid JSON with unicode sequence
    TEST_F(JsonCompressTest, CompressJson_UnicodeSequence)
    {
        const std::string inputJson = R"(
{
    "FreeTextInput": "\u003Cscript\u003Ealert(\u0022links\u0022);\u003C/script\u003E"
})";
        const std::string output    = R"({"FreeTextInput":"<script>alert(\"links\");</script>"})";
        auto              result    = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, output);
    }

    // Test 10: Test with numbers (with and without precision)
    TEST_F(JsonCompressTest, CompressJson_numbers)
    {
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

        for (const auto& [input, output] : testData)
        {
            auto result      = m_jsonHandler.GetCompressedJson(input);
            bool foundNumber = result.response.find(output) != std::string::npos;
            EXPECT_TRUE(result.success);
            EXPECT_EQ(result.error_code, -1);
            EXPECT_EQ(result.error_pos, -1);
            EXPECT_EQ(result.error_str, "");
            EXPECT_EQ(foundNumber, true) << "Number: '" << output.c_str() << "' not found in '" << result.response.c_str() << "'.\n";
        }
    }

    // Test 11: Test with different type of data
    TEST_F(JsonCompressTest, CompressJson_MultipleDataType)
    {
        const std::string inputJson = R"({
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
        const std::string outputJson
            = R"({"name":"npp-pluginList","version":"1.5.3","List":{"defaultTimeInterval":123400.0},"Array":[{"Text":"I am text","Bool":true,"Number":123456},{"Text":"","Bool":false,"Number":0,"null":null}]})";

        // Set parse options to ignore trailing commas
        ParseOptions parseOptions {.bIgnoreComment = false, .bIgnoreTrailingComma = true, .bReplaceUndefined = false};
        setParseOptions(parseOptions);

        auto result = m_jsonHandler.GetCompressedJson(inputJson);

        EXPECT_TRUE(result.success);
        EXPECT_EQ(result.error_code, -1);
        EXPECT_EQ(result.error_pos, -1);
        EXPECT_EQ(result.error_str, "");
        EXPECT_EQ(result.response, outputJson);
    }

}    // namespace JsonCompress
