#include <fstream>      // ifstream
#include <iomanip>      // std::setw
#include "parser.h"
#include "arguments.h"

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
    Checks if there is a match between the text and the pattern
    Pattern can contain a '*' wildcard symbol
    It matches any character sequence or an empty sequence
    Returns true if the text matches the pattern, returns false if it does not
*/
bool matchPattern(std::string_view text, std::string_view pattern) {
    size_t t_i = 0;
    size_t p_i = 0;
    size_t t_back = -1; // Position to reset to if mismatch of characters or end of pattern
    size_t p_back = -1; // Position to reset to if mismatch of characters
    bool wildcardFound = false;
    while(t_i < text.size()) {
        if(p_i < pattern.size() && text[t_i] == pattern[p_i]) {
            t_i++;
            p_i++;
        } else if(p_i < pattern.size() && pattern[p_i] == '*') {
            wildcardFound = true;
            p_i++;
            p_back = p_i;
            t_back = t_i;
        } else if(wildcardFound == false){
            return false;
        } else {
            p_i = p_back;
            t_back++; // Go next from the saved position
            t_i = t_back;
        }
    }
    return std::all_of(pattern.begin() + p_i, pattern.end(), [](char c) { return c == '*'; });
}

/*
    If the file name matches, returns true, otherwise false;
    If the option is empty, returns true for any .jpg file;
*/
bool matchName(const std::string& name_option, const std::string& file_name) {
    if(!name_option.empty()) {
        std::string file_name_lower = stringToLower(file_name);
        return matchPattern(file_name_lower, name_option + ".jpg");
    }
    // match any .jpg file
    return matchPattern(file_name, "*.jpg");
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