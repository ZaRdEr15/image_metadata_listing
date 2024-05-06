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
    
    Options options;
    OptionsResult result = getOptions(argc, argv, options);

    EXPECT_EQ(result, Success);
    EXPECT_EQ(options.name, "Name");
    EXPECT_EQ(options.date, "Date");
    EXPECT_EQ(options.model, "Model");
    EXPECT_EQ(options.path, "Directory");
}
