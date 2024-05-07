#include <fstream>      // ifstream
#include <iomanip>      // std::setw
#include <regex>
#include "parser.h"

// External library for EXIF metadata parsing
#include "TinyEXIF.h"

namespace fs = std::filesystem;

/*
    Shows JPEG capture date and camera model data
    If the data is empty, prints "Doesn't exist"
*/
void showData(std::string_view data, DataType type) {
    std::cout << enumToString(type) << ": ";
    int column_width = type == CaptureDate ? DATE_COLUMN_WIDTH : MODEL_COLUMN_WIDTH;
    if(!data.empty()) {
        std::cout << std::setw(column_width) << data;
    } else {
        std::cout << std::setw(column_width) << "Doesn't exist";
    }
}

/*
    Search a directory for .jpg extension files recursively and output metadata on the screen
    Return how many files where found
*/
size_t searchJPEGFiles(const Options& options) {
    size_t count = 0;
    fs::path path = fs::current_path();
    for(const auto& dir_entry : fs::recursive_directory_iterator(path)) {
        fs::path file_path = dir_entry.path();
        if(fs::is_regular_file(file_path)) {
            fs::path filename = file_path.filename();
            if(matchName(options.name, filename)) {
                std::ifstream istream(file_path, std::ifstream::binary);
                TinyEXIF::EXIFInfo imageEXIF(istream);
                if(imageEXIF.Fields) {
                    imageEXIF.DateTimeOriginal = formatDate(imageEXIF.DateTimeOriginal);
                    if(matchDate(options.date, imageEXIF.DateTimeOriginal) && matchModel(options.model, imageEXIF.Model)) {
                        count++;
                        std::cout << std::left << std::setw(FILENAME_WIDTH) << filename.generic_string();
                        showData(imageEXIF.DateTimeOriginal, CaptureDate);
                        showData(imageEXIF.Model, CameraModel);
                        std::cout << std::endl;
                    }
                } else if(options.date.empty() && options.model.empty()) {
                    count++;
                    std::cout << std::left << std::setw(FILENAME_WIDTH) << filename << "EXIF data is not available.\n";
                }
            }
        }
    }
    return count;
}

void handleDirectoryChange(const std::string& dir) {
    try {
        fs::current_path(dir);
    } catch(fs::filesystem_error const& ex) {
        std::cerr << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*
    Pattern can contain a '*' wildcard symbol
    '*' matches any character sequence or an empty sequence
*/
bool matchPattern(const std::string& text, const std::string& pattern) {
    std::regex star_replace("\\*");
    std::string wildcard_pattern = std::regex_replace(pattern, star_replace, ".*");
    std::regex wildcard_regex("^" + wildcard_pattern + "$");
    return std::regex_match(text, wildcard_regex);
}

/*
    If the file name matches, returns true, otherwise false;
    If the option is empty, returns true for any .jpg file;
*/
bool matchName(const std::string& name_option, const std::string& file_name) {
    if(!name_option.empty()) {
        std::string file_name_lower = stringToLower(file_name);
        return matchPattern(file_name_lower, name_option + "\\.jpg");
    }
    // match any .jpg file
    return matchPattern(file_name, "*\\.jpg");
}

/* 
    If the model matches, returns true, otherwise false
    If the option is empty, returns true
*/
bool matchModel(const std::string& model_option, const std::string& exif_model) {
    if(!model_option.empty()) {
        std::string exif_model_lower = stringToLower(exif_model);
        return matchPattern(exif_model_lower, model_option);
    }
    return true;
}