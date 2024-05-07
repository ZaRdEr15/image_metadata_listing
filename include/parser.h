#ifndef PARSER_H
#define PARSER_H

#include <algorithm>    // replace, transform
#include "common_includes.h"

#define MODEL_COLUMN_WIDTH 30
#define DATE_COLUMN_WIDTH 16
#define FILENAME_WIDTH 50

enum DataType {CaptureDate, CameraModel};

inline std::string enumToString(DataType type) {
    return type == CaptureDate ? "Capture Date" : "Camera Model";
}

// Used for case insensitive matches
inline std::string stringToLower(const std::string& s) {
    std::string s_copy = s;
    std::transform(s_copy.cbegin(), s_copy.cend(), s_copy.begin(), [](unsigned char c){ return std::tolower(c); });
    return s_copy;
}

size_t searchJPEGFiles(const Options& options);
void handleDirectoryChange(const std::string& dir);

#endif // PARSER_H