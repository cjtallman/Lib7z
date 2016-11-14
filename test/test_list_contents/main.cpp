
#include "gtest/gtest.h"
#include "Lib7z.h"

TEST(TestFiles, ExactlyTwo)
{
    Lib7z lib;
    Lib7z::stringlist names;
    EXPECT_EQ(2, lib.getFileNames(names, "two_text_files.7z"));
}

TEST(TestFiles, Password)
{
    Lib7z lib;
    Lib7z::stringlist names;
    EXPECT_EQ(1, lib.getFileNames(names, "one_file_password.7z", "Celery"));
}

int main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
