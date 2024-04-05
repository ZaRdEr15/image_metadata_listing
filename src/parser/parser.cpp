#include <iomanip>      // std::setw
#include "parser.h"

/*
    Shows JPEG capture date and camera model data
    If the data is empty, prints "Doesn't exist"
*/
void showData(std::string data, DataType type) {
    std::cout << enumToString(type) << ": ";
    int column_width = type == CaptureDate ? DATE_COLUMN_WIDTH : MODEL_COLUMN_WIDTH;
    if(!data.empty()) {
        std::cout << std::setw(column_width) << data;
    } else {
        std::cout << std::setw(column_width) << "Doesn't exist";
    }
}