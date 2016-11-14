
#include "gtest/gtest.h"
#include "Lib7z.h"

TEST(TestFiles, LibraryValid)
{
    Lib7z lib;
    EXPECT_TRUE(lib.libraryValid());
}

TEST(TestFiles, MissingArchive)
{
    Lib7z             lib;
    Lib7z::ArchivePtr loaded = lib.getArchive("this_archive_shouldnt_exist.7z");
    EXPECT_FALSE(loaded);
}

TEST(TestFiles, ExactlyTwo)
{
    Lib7z             lib;
    Lib7z::stringlist names;
    Lib7z::ArchivePtr loaded = lib.getArchive("two_text_files.7z");
    EXPECT_TRUE(loaded);
    EXPECT_EQ(2, lib.getFileNames(names, loaded));
}

TEST(TestFiles, Password)
{
    Lib7z             lib;
    Lib7z::stringlist names;
    Lib7z::ArchivePtr loaded = lib.getArchive("one_file_password.7z", "Celery");
    EXPECT_TRUE(loaded);
    EXPECT_EQ(1, lib.getFileNames(names, loaded));
}

TEST(TestFiles, UncompressedSize)
{
    Lib7z             lib;
    Lib7z::ArchivePtr loaded = lib.getArchive("one_text_file.7z");
    EXPECT_TRUE(loaded);
    // Get first file size uncompressed.
    EXPECT_EQ(2447699, lib.getUncompressedSize(loaded, 0));
}

TEST(TestFiles, CompressedSize)
{
    Lib7z             lib;
    Lib7z::ArchivePtr loaded = lib.getArchive("one_text_file.7z");
    EXPECT_TRUE(loaded);
    // Get first file size compressed.
    EXPECT_EQ(609269, lib.getCompressedSize(loaded, 0));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
