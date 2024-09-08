#include <gtest/gtest.h>

int wmain(int argc, wchar_t* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    int retVal = RUN_ALL_TESTS();

    return retVal;
}
