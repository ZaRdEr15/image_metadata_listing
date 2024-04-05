#include <filesystem>
#include <fstream> // ifstream

#include "TinyEXIF.h"

// Helper function headers
#include "arguments.h"
#include "parser.h"

namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 1

int main(int argc, char* argv[]) {

    std::string file_name, capture_date, camera_model, directory;
    validateOptions(argc, argv, file_name, capture_date, camera_model, directory);
    stringToLower(file_name); // case insensitive
    stringToLower(camera_model); // case insensitive

    if(DEBUG_MODE) {
        showArgs(argc, argv);
        showOptions(file_name, capture_date, camera_model);
    }

    fs::path path;
    try {
        fs::current_path(directory); // Switch to the provided directory
        path = fs::current_path();
    } catch(fs::filesystem_error const& ex) {
        std::cerr << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    

    /* for (const auto& dir_entry : fs::recursive_directory_iterator(path)) {

        // Open a stream to read just the necessary parts of the image file
        std::ifstream istream(dir_entry.path(), std::ifstream::binary);

        // Parse image EXIF and XMP metadata
        TinyEXIF::EXIFInfo imageEXIF(istream);

        if(imageEXIF.Fields) {
            std::cout << std::left << std::setw(FILENAME_WIDTH) << dir_entry.path().filename() << ": ";
            imageEXIF.DateTimeOriginal = formatDate(imageEXIF.DateTimeOriginal);
            showData(imageEXIF.DateTimeOriginal, CaptureDate);
            showData(imageEXIF.Model, CameraModel);
            std::cout << std::endl;
        }
    } */
    return 0;
}