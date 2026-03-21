// ENHANCEMENT: Add input validation and error handling

// Add before line 193:
namespace NavigationTests
{
    class NavigationTest : public ::testing::Test
    {
    protected:
        std::shared_ptr<TrackingStream> m_pTSS;

        void SetUp() override
        {
            // Setup test data
        }

        void TearDown() override
        {
            m_pTSS.reset();
        }
    };

    // Test navigation with sample positions
    TEST_F(NavigationTest, Position_Validation)
    {
        // ENHANCEMENT: Add validation for position data
        std::vector<std::pair<std::string, Position>> testPositions {
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

        // ENHANCEMENT: Validate all positions have positive values
        for (const auto& [text, pos] : testPositions)
        {
            EXPECT_GE(pos.nLine, 0) << "Invalid line number for: " << text;
            EXPECT_GE(pos.nColumn, 0) << "Invalid column for: " << text;
            EXPECT_GT(pos.nKeyLength, 0) << "Invalid key length for: " << text;
        }

        std::string jsonText = R"({...})";  // Your JSON content
        m_pTSS = std::make_shared<TrackingStream>(jsonText);
    }
}