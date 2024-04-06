#include <filesystem>
#include <fstream> // ifstream

// External library for EXIF metadata parsing
#include "TinyEXIF.h"

// Helper function headers
#include "arguments.h"
#include "parser.h"

namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 1

/*
    Search a directory for .jpg extension files recursively and output metadata on the screen
    Return how many files where found
*/
size_t searchFiles(fs::path path, std::string name_opt, std::string date_opt, std::string model_opt) {
    size_t count = 0;
    for (const auto& dir_entry : fs::recursive_directory_iterator(path)) {

        fs::path file_path = dir_entry.path();

        // Only open a stream if it is a file and not a directory
        if(fs::is_regular_file(file_path)) {

            fs::path filename = file_path.filename();

            // Parse EXIF metadata only if the name matches
            if(matchName(name_opt, filename)) {
                count++;
                // Open a stream to read just the necessary parts of the image file
                std::ifstream istream(file_path, std::ifstream::binary);

                // Parse image EXIF and XMP metadata
                TinyEXIF::EXIFInfo imageEXIF(istream);

                // If the EXIF data not empty show data
                if(imageEXIF.Fields) {
                    imageEXIF.DateTimeOriginal = formatDate(imageEXIF.DateTimeOriginal);

                    // Only if there is a match show data
                    if(matchDate(date_opt, imageEXIF.DateTimeOriginal) && matchModel(model_opt, imageEXIF.Model)) {
                        std::cout << std::left << std::setw(FILENAME_WIDTH) << filename;
                        showData(imageEXIF.DateTimeOriginal, CaptureDate);
                        showData(imageEXIF.Model, CameraModel);
                        std::cout << std::endl;
                    }
                // Even if no EXIF available, show on screen but check that date or model options were NOT provided
                // to ensure to not show file names with unavailable EXIF data
                } else if(date_opt.empty() && model_opt.empty()) {
                    std::cout << std::left << std::setw(FILENAME_WIDTH) << filename << "EXIF data is not available.\n";
                }
            }
        }
    }
    return count;
}

fs::path changeDirectory(std::string dir) {
    fs::path path;
    try {
        fs::current_path(dir); // Switch to the provided directory
        path = fs::current_path();
    } catch(fs::filesystem_error const& ex) {
        std::cerr << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return path;
}

int main(int argc, char* argv[]) {

    std::string file_name, capture_date, camera_model, directory;
    validateOptions(argc, argv, file_name, capture_date, camera_model, directory);
    stringToLower(file_name);    // case insensitive
    stringToLower(camera_model); // case insensitive

    if(DEBUG_MODE) {
        showArgs(argc, argv);
        showOptions(file_name, capture_date, camera_model);
    }

    fs::path path_to_dir = changeDirectory(directory);

    std::cout << searchFiles(path_to_dir, file_name, capture_date, camera_model) << " total found" << std::endl;

    return 0;
}