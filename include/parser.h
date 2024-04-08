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
inline std::string formatDate(std::string date) {
    if(!date.empty()) {
        date = date.erase(10);
        std::replace(date.begin(), date.end(), ':', '-');
    }
    return date;
}

void showData(std::string data, DataType type);
size_t searchJPEGFiles(std::filesystem::path path, std::string name_opt, std::string date_opt, std::string model_opt);
std::filesystem::path changeDirectory(std::string dir);

#endif // PARSER_H