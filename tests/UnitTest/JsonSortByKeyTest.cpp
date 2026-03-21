#include <gtest/gtest.h>

#include <string>

#include "JsonHandler.h"

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
        ASSERT_EQ(result.response, "{\n    \"a\": 30,\n    \"b\": \"valueB\"\n}");

        std::string inputJson2 = R"({ "name": "My name?", "age": 27, "is_student": false })";
        auto        result2    = m_jsonHandler.SortJsonByKey(inputJson2, {}, {}, ' ', 2);

        ASSERT_TRUE(result2.success);
        ASSERT_EQ(result2.response, "{\n  \"age\": 27,\n  \"is_student\": false,\n  \"name\": \"My name?\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_NaN_Value_Success)
    {
        std::string inputJson = R"({"Nan": NaN, "Hello": "World"})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 4);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n    \"Hello\": \"World\",\n    \"Nan\": NaN\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_ArrayValue_Success)
    {
        std::string inputJson = R"({"text": ["Hello", "World"], "Inf": [-Infinity, Infinity, -Infinity, Infinity]})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, LF::kFormatSingleLineArray, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"Inf\": [-Infinity, Infinity, -Infinity, Infinity],\n  \"text\": [\"Hello\", \"World\"]\n}");
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
        ASSERT_EQ(result.response, "{\n  \"a\": {\n    \"c\": 3\n  },\n  \"z\": {\n    \"a\": 1,\n    \"b\": 2\n  }\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_SpecialCharacters)
    {
        std::string inputJson = R"({"!": 1, "#": 2, "A": 3})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"!\": 1,\n  \"#\": 2,\n  \"A\": 3\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_UnicodeKeys)
    {
        std::string inputJson = R"({"α": 1, "β": 2, "γ": 3})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(result.response, "{\n  \"α\": 1,\n  \"β\": 2,\n  \"γ\": 3\n}");
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
        ASSERT_EQ(result.response, "{\n  \"a\": 1,\n  \"b\": 2\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_IgnoreTrailingComma)
    {
        ParseOptions options;
        options.bIgnoreTrailingComma = false;
        std::string inputJson        = R"({"a": 1, "b": 2,})";
        auto        result           = JsonHandler {options}.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_FALSE(result.success);
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_NumberPrecision_Success)
    {
        // Verify numbers survive the sort-by-key roundtrip without gaining quotes
        // and with full precision preserved (synced with RapidJSON's Document.RawNumberRoundtrip_Precision test).
        std::string inputJson = R"({"z": "last", "pi": 3.141592653589793238, "tiny": 1e-14, "tiny2": 0.00000000000001, "big": 12345678901234567890})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);
        ASSERT_EQ(
            result.response,
            "{\n  \"big\": 12345678901234567890,\n  \"pi\": 3.141592653589793238,\n  \"tiny\": 1e-14,\n  \"tiny2\": 0.00000000000001,\n  \"z\": \"last\"\n}");
    }

    TEST_F(JsonSortByKeyTest, TestSortJsonByKey_NumberFormats_Success)
    {
        // Verify all valid JSON number notations survive sort-by-key roundtrip exactly
        // (synced with RapidJSON's Document.RawNumberRoundtrip_NumberFormats test).
        std::string inputJson = R"({"neg_frac": -0.5,)"
                                R"( "int": 42,)"
                                R"( "neg": -17,)"
                                R"( "zero": 0,)"
                                R"( "frac": 3.14,)"
                                R"( "exp_lower": 1e10,)"
                                R"( "exp_upper": 1E10,)"
                                R"( "exp_plus": 1e+10,)"
                                R"( "exp_neg": 1e-10,)"
                                R"( "exp_frac": 1.5e3,)"
                                R"( "huge_int": 99999999999999999999,)"
                                R"( "huge_neg": -99999999999999999999,)"
                                R"( "tiny_exp": 1e-308,)"
                                R"( "tiny_frac": 0.000000000000000001,)"
                                R"( "leading_zero_frac": 0.123})";
        auto        result    = m_jsonHandler.SortJsonByKey(inputJson, {}, {}, ' ', 2);

        ASSERT_TRUE(result.success);

        // Verify exact output with alphabetically sorted keys and preserved number text
        std::string expected = "{\n"
                               "  \"exp_frac\": 1.5e3,\n"
                               "  \"exp_lower\": 1e10,\n"
                               "  \"exp_neg\": 1e-10,\n"
                               "  \"exp_plus\": 1e+10,\n"
                               "  \"exp_upper\": 1E10,\n"
                               "  \"frac\": 3.14,\n"
                               "  \"huge_int\": 99999999999999999999,\n"
                               "  \"huge_neg\": -99999999999999999999,\n"
                               "  \"int\": 42,\n"
                               "  \"leading_zero_frac\": 0.123,\n"
                               "  \"neg\": -17,\n"
                               "  \"neg_frac\": -0.5,\n"
                               "  \"tiny_exp\": 1e-308,\n"
                               "  \"tiny_frac\": 0.000000000000000001,\n"
                               "  \"zero\": 0\n"
                               "}";
        ASSERT_EQ(result.response, expected);
    }
}    // namespace JsonSortByKey
