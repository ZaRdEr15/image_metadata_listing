#include "arguments.h"
#include "parser.h"

namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 0

int main(int argc, char* argv[]) {

    if(DEBUG_MODE) {
        showArgs(argc, argv);
    }

    std::string file_name, capture_date, camera_model, directory;
    OptionsResult result = getOptions(argc, argv, file_name, capture_date, camera_model, directory);
    if(DEBUG_MODE) {
        std::cout << "Result code: " << result << std::endl;
    }
    validateOptions(result, argv[UTILITY_POS]);

    file_name = stringToLower(file_name);
    file_name = stringToLower(camera_model);

    if(DEBUG_MODE) {
        showOptions(file_name, capture_date, camera_model);
    }

    handleDirectoryChange(directory);

    std::cout << searchJPEGFiles(file_name, capture_date, camera_model) << " JPEG files found" << std::endl;

    return 0;
}