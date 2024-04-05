#include <filesystem>
#include <fstream> // ifstream
#include "TinyEXIF.h"
#include "arguments.h"
#include "parser.h"

namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 1

int main(int argc, char* argv[]) {

    std::string directory;
    std::string file_name, capture_date, camera_model;
    getOptions(argc, argv, file_name, capture_date, camera_model, directory);

    if(DEBUG_MODE) {
        showArgs(argc, argv);
        showOptions(file_name, capture_date, camera_model);
    }

    /* auto current_path = fs::current_path();

    for (const auto& dir_entry : fs::recursive_directory_iterator(current_path)) {

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