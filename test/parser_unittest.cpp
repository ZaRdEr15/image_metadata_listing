#include <gtest/gtest.h>
#include "parser.h"
#include "arguments.h"

TEST(handleDirectoryChangeTest, SuccessfulDirectoryChange) {
    std::filesystem::path expected_path = std::filesystem::current_path();
    expected_path /= "Test";

    handleDirectoryChange("Test");

    EXPECT_EQ(std::filesystem::current_path(), expected_path);
}

TEST(searchJPEGFilesTest, FileCount) {
    // Already changed to "Test" directory using previous test suite
    std::filesystem::path path = std::filesystem::current_path();
    std::cout << path << std::endl;
    int count = searchJPEGFiles("", "", "");
    EXPECT_EQ(count, 3);
}

// When throws causes the code to exit
/* TEST(changeDirectoryTest, NonExistentDirectory) {
    EXPECT_ANY_THROW(changeDirectory("Folder"));
} */