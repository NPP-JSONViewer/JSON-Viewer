#include <gtest/gtest.h>

#include <string>

#include "JsonHandler.h"

namespace JsonParsing
{
    class JsonHandlerTest : public ::testing::Test
    {
    protected:
        JsonHandler m_jsonHandler {{}};

    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

    TEST_F(JsonHandlerTest, TestGetCompressedJson_Success)
    {
        std::string inputJson = R"({"key": "value"})";
        auto        result    = m_jsonHandler.GetCompressedJson(inputJson);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, R"({"key":"value"})");
    }

    TEST_F(JsonHandlerTest, TestGetCompressedJson_InvalidJson)
    {
        std::string inputJson = R"({"key": "value")";    // Missing closing brace
        auto        result    = m_jsonHandler.GetCompressedJson(inputJson);

        ASSERT_FALSE(result.success);
        ASSERT_TRUE(result.response.empty());
    }

    TEST_F(JsonHandlerTest, TestFormatJson_Success)
    {
        std::string inputJson = R"({"key": "value"})";
        auto        result    = m_jsonHandler.FormatJson(inputJson, {}, {}, ' ', 4);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n    \"key\": \"value\"\n}");
    }

    TEST_F(JsonHandlerTest, TestFormatJson_InvalidJson)
    {
        std::string inputJson = R"({"key": "value")";    // Invalid JSON
        auto        result    = m_jsonHandler.FormatJson(inputJson, {}, {}, ' ', 4);

        ASSERT_FALSE(result.success);
    }

    // Test ValidateJson
    TEST_F(JsonHandlerTest, TestValidateJson_Success)
    {
        std::string inputJson = R"({"key": "value"})";
        auto        result    = m_jsonHandler.ValidateJson(inputJson);

        ASSERT_TRUE(result.success);
    }

    TEST_F(JsonHandlerTest, TestValidateJson_InvalidJson)
    {
        std::string inputJson = R"({"key": "value")";    // Invalid JSON
        auto        result    = m_jsonHandler.ValidateJson(inputJson);

        ASSERT_FALSE(result.success);
    }

    // Test SortJsonByKey
    TEST_F(JsonHandlerTest, TestSortJsonByKey_Success)
    {
        std::string inputJson = R"({"b": "valueB", "a": "valueA"})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 4);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n    \"a\": \"valueA\",\n    \"b\": \"valueB\"\n}");
    }

    TEST_F(JsonHandlerTest, TestSortJsonByKey_InvalidJson)
    {
        std::string inputJson = R"({"b": "valueB", "a": "valueA")";    // Invalid JSON
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 4);

        ASSERT_FALSE(result.success);
    }
}    // namespace JsonParsing
