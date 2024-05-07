#ifndef PARSER_H
#define PARSER_H

#include <algorithm>    // std::replace
#include <filesystem>
#include "common_includes.h"

#define MODEL_COLUMN_WIDTH 30
#define DATE_COLUMN_WIDTH 16
#define FILENAME_WIDTH 50

enum DataType {CaptureDate, CameraModel};

inline std::string enumToString(DataType type) {
    return type == CaptureDate ? "Capture Date" : "Camera Model";
}

// Removes timestamp (HH:MM:SS) and replaces ":" with "-"
inline std::string formatDate(const std::string& date) {
    if(date.empty()) {
        return date;
    }
    std::string date_copy = date;
    date_copy = date_copy.erase(10);
    std::replace(date_copy.begin(), date_copy.end(), ':', '-');
    return date_copy;
}

/* 
    Return true if: 
    exact match of the capture date
    date option is empty
*/
inline bool matchDate(std::string_view date_option, std::string_view exif_date) {
    if(date_option.empty()) { 
        return true; 
    }
    return (date_option == exif_date);
}

// Used for case insensitive matches
inline std::string stringToLower(const std::string& s) {
    std::string s_copy = s;
    std::transform(s_copy.cbegin(), s_copy.cend(), s_copy.begin(), [](unsigned char c){ return std::tolower(c); });
    return s_copy;
}

bool matchPattern(const std::string& text, const std::string& pattern);
bool matchName(const std::string& name_option, const std::string& file_name);
bool matchModel(const std::string& model_option, const std::string& exif_model);
size_t searchJPEGFiles(const Options& options);
void handleDirectoryChange(const std::string& dir);

#endif // PARSER_H