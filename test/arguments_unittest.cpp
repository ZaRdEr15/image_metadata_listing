#include <gtest/gtest.h>
#include "arguments.h"

TEST(toLowerTest, loweredString) {
    std::string s = "Hello, World!";
    stringToLower(s);
    EXPECT_EQ(s, "hello, world!");
}

TEST(matchDateTest, ExactMatch) {
    EXPECT_EQ(matchDate("2024-04-07", "2024-04-07"), true);
    EXPECT_EQ(matchDate("2021-01-01", "2022-01-01"), false);
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

/*
    argc and argv testing is proven to be quite difficult
    as Google Test main uses its own argc and argv 
    so not every possible case is tested for this function.
    Consider manual testing.
*/
TEST(getOptionsTest, OptionResultSuccess) {
    int argc = 8;
    char *argv[] = {"./img_metadata", "-n", "Name", "-d", "Date", "-m", "Model", "Directory"};
    std::string name, date, model, dir;

    OptionsResult result = getOptions(argc, argv, name, date, model, dir);

    EXPECT_EQ(result, Success);
    EXPECT_EQ(name, "Name");
    EXPECT_EQ(date, "Date");
    EXPECT_EQ(model, "Model");
    EXPECT_EQ(dir, "Directory");
}
