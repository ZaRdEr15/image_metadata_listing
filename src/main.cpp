#include "arguments.h"
#include "parser.h"

namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 0

int main(int argc, char* argv[]) {

    if(DEBUG_MODE) {
        showArgs(argc, argv);
    }

    Options options;
    OptionsResult result = getOptions(argc, argv, options);
    if(DEBUG_MODE) {
        std::cout << "Result code: " << result << std::endl;
    }
    validateOptions(result, argv[UTILITY_POS]);

    options.name = stringToLower(options.name);
    options.model = stringToLower(options.model);

    if(DEBUG_MODE) {
        showOptions(options);
    }

    handleDirectoryChange(options.path);

    std::cout << searchJPEGFiles(options) << " JPEG files found" << std::endl;

    return 0;
}