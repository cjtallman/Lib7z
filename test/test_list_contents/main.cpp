
#include "gtest/gtest.h"
#include "Lib7z.h"

TEST(TestFiles, ExactlyTwo)
{
    Lib7z             lib;
    Lib7z::stringlist names;
    Lib7z::ArchivePtr ar = lib.getArchive("two_text_files.7z");
    EXPECT_EQ(2, lib.getFileNames(names, ar));
}

TEST(TestFiles, Password)
{
    Lib7z             lib;
    Lib7z::stringlist names;
    Lib7z::ArchivePtr ar = lib.getArchive("one_file_password.7z", "Celery");
    EXPECT_EQ(1, lib.getFileNames(names, ar));
}

TEST(TestFiles, UncompressedSize)
{
    Lib7z             lib;
    Lib7z::ArchivePtr ar = lib.getArchive("one_text_file.7z");
    if(ar)
    {
        // Get first file size uncompressed.
        const Lib7z::ulonglong sz = lib.getUncompressedSize(ar, 0);
        EXPECT_EQ(2447699, sz);
    }
}

TEST(TestFiles, CompressedSize)
{
    Lib7z             lib;
    Lib7z::ArchivePtr ar = lib.getArchive("one_text_file.7z");
    if(ar)
    {
        // Get first file size compressed.
        const Lib7z::ulonglong sz = lib.getCompressedSize(ar, 0);
        EXPECT_EQ(609269, sz);
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
