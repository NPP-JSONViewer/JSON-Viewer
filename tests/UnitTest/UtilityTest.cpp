#include <gtest/gtest.h>

#include <windows.h>
#include <optional>
#include <filesystem>

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
