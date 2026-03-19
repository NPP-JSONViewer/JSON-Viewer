#include <gtest/gtest.h>

#include <string>

#include "JsonHandler.h"

// TODO: Adjust test case when numeric values are properly supported in SortJsonByKey

namespace JsonSortByKey
{
    class JsonSortByKeyTest : public ::testing::Test
    {
    protected:
        JsonHandler m_jsonHandler {{}};

    protected:
        void SetUp() override {}
        void TearDown() override {}
    };


    // Test SortJsonByKey
    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_Success)
    {
        std::string inputJson = R"({"b": "valueB", "a": "valueA"})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 4);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n    \"a\": \"valueA\",\n    \"b\": \"valueB\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_InvalidJson)
    {
        std::string inputJson = R"({"b": "valueB", "a": "valueA")";    // Invalid JSON
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 4);

        ASSERT_FALSE(result.success);
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_NumberValue_Success)
    {
        std::string inputJson = R"({"b": "valueB", "a": 30})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 4);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n    \"a\": \"30\",\n    \"b\": \"valueB\"\n}");

        std::string inputJson2 = R"({ "name": "My name?", "age": 27, "is_student": false })";
        auto        result2    = m_jsonHandler.SortJsonByKey(inputJson2, {}, {}, ' ', 2);

        ASSERT_TRUE(result2.success);
        ASSERT_EQ(result2.response, "{\n  \"age\": \"27\",\n  \"is_student\": false,\n  \"name\": \"My name?\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_NaN_Value_Success)
    {
        std::string inputJson = R"({"Nan": NaN, "Hello": "World"})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 4);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n    \"Hello\": \"World\",\n    \"Nan\": \"NaN\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_ArrayValue_Success)
    {
        std::string inputJson = R"({"text": ["Hello", "World"], "Inf": [-Infinity, Infinity, -Infinity, Infinity]})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, LF::kFormatSingleLineArray, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"Inf\": [\"-Infinity\", \"Infinity\", \"-Infinity\", \"Infinity\"],\n  \"text\": [\"Hello\", \"World\"]\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_EmptyObject)
    {
        std::string inputJson = R"({})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_NestedObject)
    {
        std::string inputJson = R"({"z": {"b": 2, "a": 1}, "a": {"c": 3}})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"a\": {\n    \"c\": \"3\"\n  },\n  \"z\": {\n    \"a\": \"1\",\n    \"b\": \"2\"\n  }\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_SpecialCharacters)
    {
        std::string inputJson = R"({"!": 1, "#": 2, "A": 3})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"!\": \"1\",\n  \"#\": \"2\",\n  \"A\": \"3\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_UnicodeKeys)
    {
        std::string inputJson = R"({"α": 1, "β": 2, "γ": 3})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"α\": \"1\",\n  \"β\": \"2\",\n  \"γ\": \"3\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_NullValue)
    {
        std::string inputJson = R"({"b": null, "a": "valueA"})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"a\": \"valueA\",\n  \"b\": null\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_IgnoreComment)
    {
        ParseOptions options;
        options.bIgnoreComment = true;
        std::string inputJson  = R"({"b": 2, /* comment */ "a": 1})";
        auto        result     = JsonHandler {options}.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"a\": \"1\",\n  \"b\": \"2\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_IgnoreTrailingComma)
    {
        ParseOptions options;
        options.bIgnoreTrailingComma = false;
        std::string inputJson        = R"({"a": 1, "b": 2,})";
        auto        result           = JsonHandler {options}.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_FALSE(result.success);
    }
}    // namespace JsonSortByKey
