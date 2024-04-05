#include <iomanip>      // std::setw
#include <algorithm>    // std::replace
#include "parser.h"

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