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
size_t searchJPEGFiles(std::string name_opt, std::string date_opt, std::string model_opt) {
    size_t count = 0;
    fs::path path = fs::current_path();
    for (const auto& dir_entry : fs::recursive_directory_iterator(path)) {

        fs::path file_path = dir_entry.path();

        // Only open a stream if it is a file and not a directory
        if(fs::is_regular_file(file_path)) {

            fs::path filename = file_path.filename();

            // Parse EXIF metadata only if the name matches
            if(matchName(name_opt, filename)) {
                // Open a stream to read just the necessary parts of the image file
                std::ifstream istream(file_path, std::ifstream::binary);

                // Parse image EXIF and XMP metadata
                TinyEXIF::EXIFInfo imageEXIF(istream);

                // If the EXIF data not empty show data
                if(imageEXIF.Fields) {
                    imageEXIF.DateTimeOriginal = formatDate(imageEXIF.DateTimeOriginal);

                    // Only if there is a match show data
                    if(matchDate(date_opt, imageEXIF.DateTimeOriginal) && matchModel(model_opt, imageEXIF.Model)) {
                        count++;
                        std::cout << std::left << std::setw(FILENAME_WIDTH) << filename;
                        showData(imageEXIF.DateTimeOriginal, CaptureDate);
                        showData(imageEXIF.Model, CameraModel);
                        std::cout << std::endl;
                    }
                // Even if no EXIF available, show on screen but check that date or model options were NOT provided
                // to ensure to not show file names with unavailable EXIF data
                } else if(date_opt.empty() && model_opt.empty()) {
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
    size_t i = 0; // Text index
    size_t j = 0; // Pattern index
    size_t textBacktrack = -1; // Position to reset to if mismatch of characters or end of pattern
    size_t nextToWildcard = -1; // Position to reset to if mismatch of characters
    bool wildcardFound = false;
    while(i < text.size()) {
        if(j < pattern.size() && text[i] == pattern[j]) {
            i++;
            j++;
        } else if(j < pattern.size() && pattern[j] == '*') {
            wildcardFound = true;
            j++;
            nextToWildcard = j;
            textBacktrack = i; // Save and go back from the next to it
        } else if(wildcardFound == false){
            return false; // Characters arent same and no wildcard so no match
        } else {
            // Wildcard was present, reset to next to wildcard and continue text
            j = nextToWildcard;
            textBacktrack++; // Go next from the saved position
            i = textBacktrack;
        }
    }
    // Check if all characters left are wildcard characters
    // if no there is no match
    // If the string is at the end returns true
    return std::all_of(pattern.begin() + j, pattern.end(), [](char c) { return c == '*'; });
}

/*
    Match any .jpg file depending on the wildcard symbol
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
    Match any camera model depending on the wildcard symbol
    If the model matches, returns true, otherwise false;
    If the option is empty, returns true;
*/
bool matchModel(const std::string& model_option, const std::string& exif_model) {
    if(!model_option.empty()) {
        std::string exif_model_lower = stringToLower(exif_model);
        return matchPattern(exif_model_lower, model_option);
    }
    return true;
}