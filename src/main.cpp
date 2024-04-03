#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream> // ifstream
#include "TinyEXIF.h"
#include "arguments.h"
namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 1

#define COLUMN_WIDTH 25
#define FILENAME_WIDTH 50

enum DataType {CaptureDate, CameraModel};

inline std::string enumToString(DataType type) {
    return type == CaptureDate ? "Capture Date" : "Camera Model";
}

// Removes timestamp and replaces ":" with "-"
inline std::string formatDate(std::string date) {
    if(!date.empty()) {
        date = date.erase(10);
        std::replace(date.begin(), date.end(), ':', '-');
    }
    return date;
}

/*
    Shows JPEG capture date and camera model data
    If the data is empty, prints "Doesn't exist"
*/
void showData(std::string data, DataType type) {
    std::cout << enumToString(type) << ": ";
    if(!data.empty()) {
        std::cout << std::setw(COLUMN_WIDTH) << data;
    } else {
        std::cout << std::setw(COLUMN_WIDTH) << "Doesn't exist";
    }
}

int main(int argc, char* argv[]) {

    if(DEBUG_MODE) {
        showArgs(argc, argv);
    }

    auto current_path = fs::current_path();

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
    }
    return 0;
}