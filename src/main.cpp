// Helper function headers
#include "arguments.h"
#include "parser.h"

namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 1

int main(int argc, char* argv[]) {

    if(DEBUG_MODE) {
        showArgs(argc, argv);
    }

    std::string file_name, capture_date, camera_model, directory;
    validateOptions(argc, argv, file_name, capture_date, camera_model, directory);
    stringToLower(file_name);    // case insensitive
    stringToLower(camera_model); // case insensitive

    if(DEBUG_MODE) {
        showOptions(file_name, capture_date, camera_model);
    }

    fs::path path_to_dir = changeDirectory(directory);

    std::cout << searchFiles(path_to_dir, file_name, capture_date, camera_model) << " JPEG files found" << std::endl;

    return 0;
}