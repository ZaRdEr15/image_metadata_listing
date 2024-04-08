#include <gtest/gtest.h>
#include "parser.h"

TEST(changeDirectoryTest, SuccessfulDirectoryChange) {
    std::filesystem::path expected_path = std::filesystem::current_path();
    expected_path /= "Test";

    EXPECT_EQ(changeDirectory("Test"), expected_path);
}

TEST(searchJPEGFilesTest, FileCount) {
    std::filesystem::path path = std::filesystem::current_path(); // Already changed to "Test" directory
    std::cout << path << std::endl;
    int count = searchJPEGFiles(path, "", "", "");
    EXPECT_EQ(count, 3);
}

// When throws causes the code to exit
/* TEST(changeDirectoryTest, NonExistentDirectory) {
    EXPECT_ANY_THROW(changeDirectory("Folder"));
} */