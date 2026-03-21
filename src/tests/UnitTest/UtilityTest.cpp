// ENHANCEMENT: Add missing includes and improve test organization

#include <gtest/gtest.h>
#include "Utility.h"
#include <filesystem>
#include <fstream>

namespace UtilityTests
{
    class CUtilityTest : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Create temporary test file for file operations
            m_tempFilePath = std::filesystem::temp_directory_path() / "test_utility.tmp";
            std::ofstream testFile(m_tempFilePath);
            testFile << "test content";
            testFile.close();
        }

        void TearDown() override
        {
            // Clean up temporary test file
            if (std::filesystem::exists(m_tempFilePath))
            {
                std::filesystem::remove(m_tempFilePath);
            }
        }

        std::filesystem::path m_tempFilePath;
    };

    // Test GetXFromLPARAM with sample LPARAM
    TEST_F(CUtilityTest, GetXFromLPARAM)
    {
        LPARAM lp = MAKELPARAM(50, 100);
        short x = CUtility::GetXFromLPARAM(lp);
        EXPECT_EQ(x, 50);
    }

    // Test GetYFromLPARAM with sample LPARAM
    TEST_F(CUtilityTest, GetYFromLPARAM)
    {
        LPARAM lp = MAKELPARAM(50, 100);
        short y = CUtility::GetYFromLPARAM(lp);
        EXPECT_EQ(y, 100);
    }

    // Test DirExist with a valid directory
    TEST_F(CUtilityTest, DirExist_ValidDir)
    {
        bool exists = CUtility::DirExist(L"C:\\Windows");
        EXPECT_TRUE(exists);
    }

    // Test DirExist with an invalid directory
    TEST_F(CUtilityTest, DirExist_InvalidDir)
    {
        bool exists = CUtility::DirExist(L"Z:\\InvalidDir_12345");
        EXPECT_FALSE(exists);
    }

    // Test FileExist with a valid file
    TEST_F(CUtilityTest, FileExist_ValidFile)
    {
        bool exists = CUtility::FileExist(m_tempFilePath.wstring());
        EXPECT_TRUE(exists);
    }

    // Test FileExist with an invalid file
    TEST_F(CUtilityTest, FileExist_InvalidFile)
    {
        bool exists = CUtility::FileExist(L"C:\\InvalidPath_99999\\file.exe");
        EXPECT_FALSE(exists);
    }

    // Test FileSize with a valid file
    TEST_F(CUtilityTest, FileSize_ValidFile)
    {
        long size = CUtility::FileSize(m_tempFilePath.wstring());
        EXPECT_GT(size, 0);
    }

    // Test FileSize with an invalid file
    TEST_F(CUtilityTest, FileSize_InvalidFile)
    {
        long size = CUtility::FileSize(L"C:\\NonExistent\\file.tmp");
        EXPECT_EQ(size, -1);
    }

    // Test CreateDir functionality
    TEST_F(CUtilityTest, CreateDir_Success)
    {
        std::wstring testDir = std::filesystem::temp_directory_path().wstring() + L"\\TestDir_Utility";
        bool created = CUtility::CreateDir(testDir);
        EXPECT_TRUE(created);
        
        // Cleanup
        if (std::filesystem::exists(testDir))
        {
            std::filesystem::remove(testDir);
        }
    }

    // Test Copy functionality
    TEST_F(CUtilityTest, Copy_Success)
    {
        auto dstFile = std::filesystem::temp_directory_path() / "test_copy.tmp";
        bool copied = CUtility::Copy(m_tempFilePath.wstring(), dstFile.wstring());
        EXPECT_TRUE(copied);
        EXPECT_TRUE(std::filesystem::exists(dstFile));
        
        // Cleanup
        if (std::filesystem::exists(dstFile))
        {
            std::filesystem::remove(dstFile);
        }
    }

    // Test GetNumber with valid number
    TEST_F(CUtilityTest, GetNumber_ValidNumber)
    {
        auto result = CUtility::GetNumber(L"12345");
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 12345);
    }

    // Test GetNumber with invalid input
    TEST_F(CUtilityTest, GetNumber_InvalidInput)
    {
        auto result = CUtility::GetNumber(L"not_a_number");
        EXPECT_FALSE(result.has_value());
    }

    // Test GetNumber with negative number
    TEST_F(CUtilityTest, GetNumber_NegativeNumber)
    {
        auto result = CUtility::GetNumber(L"-999");
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), -999);
    }

    // Test GetEditCtrlText with nullptr
    TEST_F(CUtilityTest, GetEditCtrlText_NullHandle)
    {
        auto text = CUtility::GetEditCtrlText(nullptr);
        EXPECT_EQ(text, L"");
    }

    // Test GetCheckboxStatus with nullptr
    TEST_F(CUtilityTest, GetCheckboxStatus_NullHandle)
    {
        bool status = CUtility::GetCheckboxStatus(nullptr);
        EXPECT_FALSE(status);
    }
}