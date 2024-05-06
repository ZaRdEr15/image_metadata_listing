#include <gtest/gtest.h>
#include "parser.h"

TEST(matchDateTest, ExactMatch) {
    EXPECT_EQ(matchDate("2024-04-07", "2024-04-07"), true);
    EXPECT_EQ(matchDate("2021-01-01", "2022-01-01"), false);
}

TEST(toLowerTest, loweredString) {
    std::string s = "Hello, World!";
    s = stringToLower(s);
    EXPECT_EQ(s, "hello, world!");
}

TEST(matchPatternTest, ExactMatch) {
    EXPECT_EQ(matchPattern("Camera Model", "Camera Model"), true);
    EXPECT_EQ(matchPattern("Exactmatch", "Exact"), false);
}

TEST(matchPatternTest, PartialMatchAtTheStart) {
    EXPECT_EQ(matchPattern("iPhone X", "*X"), true);
    EXPECT_EQ(matchPattern("camera_lens_test.jpg", "****test.jpg"), true);
    EXPECT_EQ(matchPattern("notes.txt", "*.jpg"), false);
    EXPECT_EQ(matchPattern("testing", "*ting"), true);
    EXPECT_EQ(matchPattern("problem.jpg", "*b.jpg"), false);
    
}

TEST(matchPatternTest, PartialMatchAny) {
    EXPECT_EQ(matchPattern("VeryBigText", "*"), true);
}

TEST(matchPatternTest, PartialMatchAtTheEnd) {
    EXPECT_EQ(matchPattern("Camera Canon", "Camera*"), true);
    EXPECT_EQ(matchPattern("photo2024.jpg", "photo*****"), true);
    EXPECT_EQ(matchPattern("photo2024.jpg", "photo*****s"), false);
    EXPECT_EQ(matchPattern("Xiamoi", "Xa*"), false);
}

TEST(matchPatternTest, PartialMatchInTheMiddle) {
    EXPECT_EQ(matchPattern("iPhone", "iP*e"), true);
    EXPECT_EQ(matchPattern("Polaroid", "P***a***d"), true);
    EXPECT_EQ(matchPattern("Polaroid", "P***ol***d"), true);
    EXPECT_EQ(matchPattern("NewCamera", "N*C*r*ta"), false);
    EXPECT_EQ(matchPattern("aaab", "a*c"), false);
    EXPECT_EQ(matchPattern("aabdcbc", "a*bc"), true);
    EXPECT_EQ(matchPattern("aabcccc", "a*b*c"), true);
}

TEST(matchPatternTest, PartialMatchMix) {
    EXPECT_EQ(matchPattern("Sony", "*o*"), true);
    EXPECT_EQ(matchPattern("Lumix", "*c*"), false);
    EXPECT_EQ(matchPattern("Casio", "*a*i*"), true);
    EXPECT_EQ(matchPattern("aabcc", "a*cb*"), false);
}

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
    Options options;
    options.name = "";
    options.date = "";
    options.model = "";
    int count = searchJPEGFiles(options);
    EXPECT_EQ(count, 3);
}

// When throws causes the code to exit
/* TEST(changeDirectoryTest, NonExistentDirectory) {
    EXPECT_ANY_THROW(changeDirectory("Folder"));
} */