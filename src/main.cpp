#include <filesystem>
#include <fstream> // ifstream
#include <regex>

// External library for EXIF metadata parsing
#include "TinyEXIF.h"

// Helper function headers
#include "arguments.h"
#include "parser.h"

namespace fs = std::filesystem; // for convenience

#define DEBUG_MODE 1

bool matchDate(std::string date_option, std::string exif_date) {
    // If the option is empty return true
    bool match = true;
    if(!date_option.empty()) { // if there is a capture date option, then check if there is a match
        match = false;
        // Exact date match
        if(date_option == exif_date) {
            match = true;
        }
    }
    return match;
}

// Remove a wildcard to allow to create regex pattern
inline std::string removeWildcard(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '*'), s.end());
    return s;
}

std::string getMatchPattern(std::string opt, std::string regex) {
    if(countWildcard(opt)) { // if there is a wildcard symbol
        if(opt[0] == '*') {
            if(opt.size() == 1) {
                return regex + "+"; // "*" as option
            }
            return regex + "*" + removeWildcard(opt) + "$"; // "*abc" as option
        }
        return "^" + removeWildcard(opt) + regex + "*"; // "abc*" as option
    }
    return "^" + opt + "$"; // exact match as there is no wildcard symbols
}

bool matchName(std::string name_option, std::string file_name) {
    // If the option is empty return true
    bool match = true;
    if(!name_option.empty()) { // if there is a camera model option, then check if there is a match
        match = false;
        // Set regex according to wildcard symbol
        std::string regex_pattern = getMatchPattern(name_option, "[\\w]"); // matches a-zA-Z0-9_
        regex_pattern += "\\.jpg"; // matches for any .jpg file
        std::cout << regex_pattern << std::endl; // DEBUG

        std::regex model_regex(regex_pattern);
        
        stringToLower(file_name);
        match = std::regex_match(file_name, model_regex);
    }
    return match;
}

bool matchModel(std::string model_option, std::string exif_model) {
    // If the option is empty return true
    bool match = true;
    if(!model_option.empty()) { // if there is a camera model option, then check if there is a match
        match = false;
        // Set regex according to wildcard symbol
        std::string regex_pattern = getMatchPattern(model_option, "[\\w ]"); // matches a-zA-Z0-9_ and a whitespace

        std::regex model_regex(regex_pattern);
        
        stringToLower(exif_model);
        match = std::regex_match(exif_model, model_regex);
    }
    return match;
}

/*
    Search a directory for .jpg extension files recursively and output metadata on the screen
    Return how many files where found
*/
size_t searchFiles(fs::path path, std::string capture_date, std::string camera_model) {
    size_t count = 0;
    for (const auto& dir_entry : fs::recursive_directory_iterator(path)) {
        // Only open a stream if it is a file and not a directory
        if(fs::is_regular_file(dir_entry.path())) {
            // Open a stream to read just the necessary parts of the image file
            std::ifstream istream(dir_entry.path(), std::ifstream::binary);

            // Parse image EXIF and XMP metadata
            TinyEXIF::EXIFInfo imageEXIF(istream);

            // If the EXIF data not empty show data
            if(imageEXIF.Fields) {
                imageEXIF.DateTimeOriginal = formatDate(imageEXIF.DateTimeOriginal);

                // Only if there is a match show data
                if(matchDate(capture_date, imageEXIF.DateTimeOriginal) && matchModel(camera_model, imageEXIF.Model)) {
                    count++;
                    std::cout << std::left << std::setw(FILENAME_WIDTH) << dir_entry.path().filename();
                    showData(imageEXIF.DateTimeOriginal, CaptureDate);
                    showData(imageEXIF.Model, CameraModel);
                    std::cout << std::endl;
                }
            } /* else { // only in case of file matching
                std::cout << std::left << std::setw(FILENAME_WIDTH) << dir_entry.path().filename() << "EXIF data fields unavailable.\n";
                count++;
            } */
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

    fs::path path = changeDirectory(directory);

    std::cout << searchFiles(path, capture_date, camera_model) << " total found" << std::endl;

    return 0;
}