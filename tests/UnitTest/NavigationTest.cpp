#include <gtest/gtest.h>

#include <unordered_map>
#include <optional>
#include <memory>

#include "TreeHandler.h"
#include "JsonHandler.h"
#include "TrackingStream.h"
#include "RapidJsonHandler.h"


class TreeHandlerTest : public TreeHandler
{
    std::unordered_map<std::string, Position> m_mapKeyPosition;

public:
    virtual ~TreeHandlerTest() = default;


    // Inherited via TreeHandler
    HTREEITEM InsertToTree(HTREEITEM /*parent*/, const std::string& text) override
    {
        m_mapKeyPosition[text] = {};
        return HTREEITEM();
    }

    HTREEITEM InsertToTree(HTREEITEM /*parent*/, const std::string& text, const Position& pos) override
    {
        m_mapKeyPosition[text] = pos;
        return HTREEITEM();
    }

    void AppendNodeCount(HTREEITEM /*node*/, unsigned /*elementCount*/, bool /*bArray*/) override {}

    auto GetPosition(const std::string& text) const -> std::optional<Position>
    {
        auto find = m_mapKeyPosition.find(text);

        std::optional<Position> retVal = std::nullopt;
        if (find != m_mapKeyPosition.cend())
            retVal = find->second;

        return retVal;
    }
};

namespace JsonNavigation
{
    using KeyPos = std::pair<std::string, Position>;
    struct TestData
    {
        std::string         input;
        std::vector<KeyPos> output;
    };

    class NavigationTest : public ::testing::Test
    {
    protected:
        TestData                          m_testData;
        JsonHandler                       m_jsonHandler {{}};
        TreeHandlerTest                   m_TreeHandler;
        TrackingStreamSharedPtr           m_pTSS = nullptr;
        std::unique_ptr<RapidJsonHandler> m_pHandler;
    };

    TEST_F(NavigationTest, StringKey)
    {
        const std::string jsonText = R"({
    "key1" : "value1",
    "key2" : "value2",
    "keyLong" : "Value very very long",
    "k" : "V"
})";

        m_testData.input  = jsonText;
        m_testData.output = {
            {R"(key1 : "value1")", Position {.nLine = 1, .nColumn = 5, .nKeyLength = 4}},
            {R"(key2 : "value2")", Position {.nLine = 2, .nColumn = 5, .nKeyLength = 4}},
            {R"(keyLong : "Value very very long")", Position {.nLine = 3, .nColumn = 5, .nKeyLength = 7}},
            {R"(k : "V")", Position {.nLine = 4, .nColumn = 5, .nKeyLength = 1}},
        };

        m_pTSS     = std::make_shared<TrackingStream>(jsonText);
        m_pHandler = std::make_unique<RapidJsonHandler>(&m_TreeHandler, nullptr, m_pTSS);
        rapidjson::StringBuffer sb;

        Result res = m_jsonHandler.ParseJson<flgBaseReader>(jsonText, sb, *m_pHandler, m_pTSS);
        ASSERT_TRUE(res.success);

        for (const auto& each : m_testData.output)
        {
            const auto pos = m_TreeHandler.GetPosition(each.first);
            ASSERT_TRUE(pos.has_value()) << "For key: " << each.first << std::endl;

            auto posValue = pos.value();
            ASSERT_EQ(posValue.nLine, each.second.nLine) << "For key: " << each.first << std::endl;
            ASSERT_EQ(posValue.nColumn, each.second.nColumn) << "For key: " << each.first << std::endl;
            ASSERT_EQ(posValue.nKeyLength, each.second.nKeyLength) << "For key: " << each.first << std::endl;
        }
    }

    TEST_F(NavigationTest, StringKeyCompressed)
    {
        const std::string jsonText = R"({"key1":"value1","key2":"value2","keyLong":"Value very very long","k":"V"})";

        m_testData.input  = jsonText;
        m_testData.output = {
            {R"(key1 : "value1")", Position {.nLine = 0, .nColumn = 2, .nKeyLength = 4}},
            {R"(key2 : "value2")", Position {.nLine = 0, .nColumn = 18, .nKeyLength = 4}},
            {R"(keyLong : "Value very very long")", Position {.nLine = 0, .nColumn = 34, .nKeyLength = 7}},
            {R"(k : "V")", Position {.nLine = 0, .nColumn = 67, .nKeyLength = 1}},
        };

        m_pTSS     = std::make_shared<TrackingStream>(jsonText);
        m_pHandler = std::make_unique<RapidJsonHandler>(&m_TreeHandler, nullptr, m_pTSS);
        rapidjson::StringBuffer sb;

        Result res = m_jsonHandler.ParseJson<flgBaseReader>(jsonText, sb, *m_pHandler, m_pTSS);
        ASSERT_TRUE(res.success);

        for (const auto& each : m_testData.output)
        {
            const auto pos = m_TreeHandler.GetPosition(each.first);
            ASSERT_TRUE(pos.has_value()) << "For key: " << each.first << std::endl;

            auto posValue = pos.value();
            ASSERT_EQ(posValue.nLine, each.second.nLine) << "For key: " << each.first << std::endl;
            ASSERT_EQ(posValue.nColumn, each.second.nColumn) << "For key: " << each.first << std::endl;
            ASSERT_EQ(posValue.nKeyLength, each.second.nKeyLength) << "For key: " << each.first << std::endl;
        }
    }

    TEST_F(NavigationTest, MixedKeys)
    {
        const std::string jsonText = R"({
  // this is comment
  "Id": 100000000302052988.0,
  "num": [12.148681171238422,42.835353759876654],
  "timeInfo":1.234e5,
  "FreeTextInput":"\u003Cscript\u003Ealert(\u0022links\u0022);\u003C/script\u003E",
  "text1":["Hello", "World"],
  "text2":[true, false, true],
  "text3":[null, null, null, "power"],
  "Test":[
    {
      "ok": [
        "HelloWorld"
      ]
    },
    {
      "Tata": [
        "TataByeBye"
      ]
    }
  ],
  "Nan": NaN,
  "Inf":[
    -Infinity,
    Infinity,
    -Infinity,
    Infinity
  ],
  "Object":{"Test1":"Test1", "Test2":"Test2"}
})";

        m_testData.input  = jsonText;
        m_testData.output = {
            {R"(Id : 100000000302052988.0)", Position {.nLine = 2, .nColumn = 3, .nKeyLength = 2}},
            {R"(num)", Position {.nLine = 3, .nColumn = 3, .nKeyLength = 3}},
            {R"([0] : 12.148681171238422)", Position {.nLine = 3, .nColumn = 10, .nKeyLength = 18}},
            {R"([1] : 42.835353759876654)", Position {.nLine = 3, .nColumn = 29, .nKeyLength = 18}},
            {R"(timeInfo : 1.234e5)", Position {.nLine = 4, .nColumn = 3, .nKeyLength = 8}},
            {R"(FreeTextInput : "<script>alert("links");</script>")", Position {.nLine = 5, .nColumn = 3, .nKeyLength = 13}},

            {R"(text1)", Position {.nLine = 6, .nColumn = 3, .nKeyLength = 5}},
            {R"([0] : "Hello")", Position {.nLine = 6, .nColumn = 12, .nKeyLength = 5}},
            {R"([1] : "World")", Position {.nLine = 6, .nColumn = 21, .nKeyLength = 5}},

            {R"(text2)", Position {.nLine = 7, .nColumn = 3, .nKeyLength = 5}},
            {R"([0] : true)", Position {.nLine = 7, .nColumn = 11, .nKeyLength = 4}},
            {R"([1] : false)", Position {.nLine = 7, .nColumn = 17, .nKeyLength = 5}},
            {R"([2] : true)", Position {.nLine = 7, .nColumn = 24, .nKeyLength = 4}},

            {R"(text3)", Position {.nLine = 8, .nColumn = 3, .nKeyLength = 5}},
            {R"([0] : null)", Position {.nLine = 8, .nColumn = 11, .nKeyLength = 4}},
            {R"([1] : null)", Position {.nLine = 8, .nColumn = 17, .nKeyLength = 4}},
            {R"([2] : null)", Position {.nLine = 8, .nColumn = 23, .nKeyLength = 4}},
            {R"([3] : "power")", Position {.nLine = 8, .nColumn = 30, .nKeyLength = 5}},

            {R"(Test)", Position {.nLine = 9, .nColumn = 3, .nKeyLength = 4}},
            {R"(ok)", Position {.nLine = 11, .nColumn = 7, .nKeyLength = 2}},
            {R"([0] : "HelloWorld")", Position {.nLine = 12, .nColumn = 9, .nKeyLength = 10}},
            {R"(Tata)", Position {.nLine = 16, .nColumn = 7, .nKeyLength = 4}},
            {R"([0] : "TataByeBye")", Position {.nLine = 17, .nColumn = 9, .nKeyLength = 10}},

            {R"(Nan : NaN)", Position {.nLine = 21, .nColumn = 3, .nKeyLength = 3}},

            {R"(Inf)", Position {.nLine = 22, .nColumn = 3, .nKeyLength = 3}},
            {R"([0] : -Infinity)", Position {.nLine = 23, .nColumn = 4, .nKeyLength = 9}},
            {R"([1] : Infinity)", Position {.nLine = 24, .nColumn = 4, .nKeyLength = 8}},
            {R"([2] : -Infinity)", Position {.nLine = 25, .nColumn = 4, .nKeyLength = 9}},
            {R"([3] : Infinity)", Position {.nLine = 26, .nColumn = 4, .nKeyLength = 8}},

            {R"(Object)", Position {.nLine = 28, .nColumn = 3, .nKeyLength = 6}},
            {R"(Test1 : "Test1")", Position {.nLine = 28, .nColumn = 13, .nKeyLength = 5}},
            {R"(Test2 : "Test2")", Position {.nLine = 28, .nColumn = 30, .nKeyLength = 5}},
        };

        m_pTSS     = std::make_shared<TrackingStream>(jsonText);
        m_pHandler = std::make_unique<RapidJsonHandler>(&m_TreeHandler, nullptr, m_pTSS);
        rapidjson::StringBuffer sb;

        Result res = m_jsonHandler.ParseJson<flgBaseReader>(jsonText, sb, *m_pHandler, m_pTSS);
        ASSERT_TRUE(res.success);

        for (const auto& each : m_testData.output)
        {
            const auto pos = m_TreeHandler.GetPosition(each.first);
            ASSERT_TRUE(pos.has_value()) << "For key: " << each.first << std::endl;

            auto posValue = pos.value();
            ASSERT_EQ(posValue.nLine, each.second.nLine) << "For key: " << each.first << std::endl;
            ASSERT_EQ(posValue.nColumn, each.second.nColumn) << "For key: " << each.first << std::endl;
            ASSERT_EQ(posValue.nKeyLength, each.second.nKeyLength) << "For key: " << each.first << std::endl;
        }
    }
}    // namespace JsonNavigation
