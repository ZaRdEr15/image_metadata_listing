#include <fstream>      // ifstream
#include <iomanip>      // setw
#include <regex>        // regex_replace, regex_match
#include "parser.h"

// External library for EXIF metadata parsing
#include "TinyEXIF.h"

namespace fs = std::filesystem;

void formatPrint(const std::string& data) {
    const std::string emptyData = "No data";
    const std::string dataToPrint = data.empty() ? emptyData : data;
    int columnWidth = dataToPrint.size() + COLUMN_SPACING;
    std::cout << std::left << std::setw(columnWidth) << dataToPrint;
}

/*
    Shows JPEG capture date and camera model data
    If the data is empty, prints "Doesn't exist"
*/
void showData(std::string_view data) {
    // ????
}

// Removes timestamp (HH:MM:SS) and replaces ":" with "-"
inline std::string formatDate(const std::string& date) {
    if(date.empty()) {return date;}
    std::string date_copy = date;
    date_copy = date_copy.erase(10);
    std::replace(date_copy.begin(), date_copy.end(), ':', '-');
    return date_copy;
}

TinyEXIF::EXIFInfo getEXIFInfo(const fs::path& file_path) {
    std::ifstream istream(file_path, std::ifstream::binary);
    TinyEXIF::EXIFInfo imageEXIF(istream);
    return imageEXIF;
}

// Returns 1 if data matches the options, otherwise 0
size_t parseEXIF(const fs::path& file_path, const Options& options) {
    std::string filename = file_path.filename().generic_string();
    if(matchName(options.name, filename)) {
        TinyEXIF::EXIFInfo imageEXIF = getEXIFInfo(file_path);
        if(imageEXIF.Fields) {
            imageEXIF.DateTimeOriginal = formatDate(imageEXIF.DateTimeOriginal);
            if(matchDate(options.date, imageEXIF.DateTimeOriginal) && 
               matchModel(options.model, imageEXIF.Model)) {
                formatPrint(imageEXIF.DateTimeOriginal);
                formatPrint(imageEXIF.Model);
                formatPrint(filename);
                std::cout << std::endl;
                return 1;
            }
        } else if(options.date.empty() && options.model.empty()) {
            formatPrint(filename); 
            std::cout << "EXIF data is unavailable.\n";
            return 1;
        }
    }
    return 0;
}

/*
    Search a directory for .jpg extension files recursively and output metadata on the screen
    Return how many files where found
*/
size_t searchJPEGFiles(const Options& options) {
    size_t count = 0;
    for(const auto& dir_entry : fs::recursive_directory_iterator(fs::current_path())) {
        const fs::path file_path = dir_entry.path();
        if(fs::is_regular_file(file_path)) {
            count += parseEXIF(file_path, options);
        }
        // Show some error if unable to read file!!!!!
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
    Return true if: 
    exact match of the capture date
    date option is empty
*/
inline bool matchDate(std::string_view date_option, std::string_view exif_date) {
    if(date_option.empty()) {return true;}
    return (date_option == exif_date);
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