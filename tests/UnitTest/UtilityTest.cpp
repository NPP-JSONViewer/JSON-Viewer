#include <gtest/gtest.h>

#include <windows.h>
#include <optional>
#include <filesystem>
#include <fstream>

#include "Utility.h"

namespace UtilityTest
{
    // Test GetXFromLPARAM with sample LPARAM
    TEST(CUtilityTest, GetXFromLPARAM)
    {
        CUtility util;
        LPARAM   lp = MAKELPARAM(50, 100);
        short    x  = util.GetXFromLPARAM(lp);
        EXPECT_EQ(x, 50);
    }

    // Test GetYFromLPARAM with sample LPARAM
    TEST(CUtilityTest, GetYFromLPARAM)
    {
        CUtility util;
        LPARAM   lp = MAKELPARAM(50, 100);
        short    y  = util.GetYFromLPARAM(lp);
        EXPECT_EQ(y, 100);
    }

    // Test GetEditCtrlText with nullptr
    TEST(CUtilityTest, GetEditCtrlText_NullHandle)
    {
        auto text = CUtility::GetEditCtrlText(nullptr);
        EXPECT_EQ(text, L"");
    }

    // Test GetCheckboxStatus with nullptr
    TEST(CUtilityTest, GetCheckboxStatus_NullHandle)
    {
        bool status = CUtility::GetCheckboxStatus(nullptr);
        EXPECT_FALSE(status);
    }

    // Test CreateDir functionality
    TEST(CUtilityTest, CreateDir_Success)
    {
        std::wstring testDir = std::filesystem::temp_directory_path().wstring() + L"\\TestDir_Utility";
        bool         created = CUtility::CreateDir(testDir);
        EXPECT_TRUE(created);

        // Cleanup
        if (std::filesystem::exists(testDir))
        {
            std::filesystem::remove(testDir);
        }
    }

    // Test Copy functionality
    TEST(CUtilityTest, Copy_Success)
    {
        std::filesystem::path srcFile = std::filesystem::temp_directory_path() / "test_utility.tmp";
        auto                  dstFile = std::filesystem::temp_directory_path() / "test_copy.tmp";

        std::ofstream testFile(srcFile);
        testFile << "test content";
        testFile.close();

        if (!std::filesystem::exists(srcFile))
        {
            // Something bad, failed to create file, hence skip the test
            GTEST_SKIP() << "Failed to create source file for copy test.";
        }

        bool copied = CUtility::Copy(srcFile.wstring(), dstFile.wstring());
        EXPECT_TRUE(copied);
        EXPECT_TRUE(std::filesystem::exists(dstFile));

        // Cleanup
        if (std::filesystem::exists(srcFile))
        {
            std::filesystem::remove(srcFile);
        }
        if (std::filesystem::exists(dstFile))
        {
            std::filesystem::remove(dstFile);
        }
    }

    // Test DirExist with a valid directory
    TEST(CUtilityTest, DirExist_ValidDir)
    {
        CUtility util;
        bool     exists = util.DirExist(L"C:\\Windows");
        EXPECT_TRUE(exists);
    }

    // Test DirExist with an invalid directory
    TEST(CUtilityTest, DirExist_InvalidDir)
    {
        CUtility util;
        bool     exists = util.DirExist(L"Z:\\InvalidDir");
        EXPECT_FALSE(exists);
    }

    // Test FileExist with a valid file
    TEST(CUtilityTest, FileExist_ValidFile)
    {
        CUtility util;
        bool     exists = util.FileExist(L"C:\\Windows\\notepad.exe");
        EXPECT_TRUE(exists);
    }

    // Test FileExist with an invalid file
    TEST(CUtilityTest, FileExist_InvalidFile)
    {
        CUtility util;
        bool     exists = util.FileExist(L"C:\\InvalidPath\\file.exe");
        EXPECT_FALSE(exists);
    }

    // Test FileSize with a valid file
    TEST(CUtilityTest, FileSize_ValidFile)
    {
        CUtility util;
        long     size = util.FileSize(L"C:\\Windows\\notepad.exe");
        EXPECT_GT(size, 0);
    }

    // Test FileSize with an invalid file
    TEST(CUtilityTest, FileSize_InvalidFile)
    {
        CUtility util;
        EXPECT_THROW(util.FileSize(L"C:\\InvalidPath\\file.exe"), std::filesystem::filesystem_error);
    }

    // Test GetFileName with extension
    TEST(CUtilityTest, GetFileName_WithExtension)
    {
        CUtility     util;
        std::wstring fileName = util.GetFileName(L"C:\\path\\file.txt", true);
        EXPECT_EQ(fileName, L"file.txt");
    }

    // Test GetFileName without extension
    TEST(CUtilityTest, GetFileName_WithoutExtension)
    {
        CUtility     util;
        std::wstring fileName = util.GetFileName(L"C:\\path\\file.txt", false);
        EXPECT_EQ(fileName, L"file");
    }

    // Test GetFileExtension with a valid file name
    TEST(CUtilityTest, GetFileExtension_ValidFileName)
    {
        CUtility     util;
        std::wstring ext = util.GetFileExtension(L"file.txt");
        EXPECT_EQ(ext, L".txt");
    }

    // Test GetTempFilePath
    TEST(CUtilityTest, GetTempFilePath)
    {
        CUtility     util;
        std::wstring tempPath = util.GetTempFilePath();
        EXPECT_FALSE(tempPath.empty());
    }

    // Test IsNumber with a valid number string
    TEST(CUtilityTest, IsNumber_ValidNumber)
    {
        CUtility util;
        bool     isNumber = util.IsNumber(L"12345");
        EXPECT_TRUE(isNumber);
    }

    // Test IsNumber with an invalid number string
    TEST(CUtilityTest, IsNumber_InvalidNumber)
    {
        CUtility util;
        bool     isNumber = util.IsNumber(L"123a45");
        EXPECT_FALSE(isNumber);
    }

    // Test GetNumber with valid number string
    TEST(CUtilityTest, GetNumber_ValidNumber)
    {
        CUtility util;
        auto     number = util.GetNumber(L"12345");
        ASSERT_TRUE(number.has_value());
        EXPECT_EQ(number.value(), 12345);
    }

    // Test GetNumber with invalid number string
    TEST(CUtilityTest, GetNumber_InvalidNumber)
    {
        CUtility util;
        auto     number = util.GetNumber(L"123a45");
        EXPECT_FALSE(number.has_value());
    }

}    // namespace UtilityTest
