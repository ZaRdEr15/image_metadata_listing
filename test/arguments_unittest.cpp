#include <gtest/gtest.h>
#include "arguments.h"

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
