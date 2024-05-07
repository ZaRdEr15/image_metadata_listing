#ifndef PARSER_H
#define PARSER_H

#include <filesystem>
#include <algorithm>    // replace, transform
#include "common_includes.h"

#define COLUMN_SPACING 2

// Used for case insensitive matches
inline std::string stringToLower(const std::string& s) {
    std::string s_copy = s;
    std::transform(s_copy.cbegin(), s_copy.cend(), s_copy.begin(), [](unsigned char c){ return std::tolower(c); });
    return s_copy;
}

bool matchDate(std::string_view date_option, std::string_view exif_date);
bool matchName(const std::string& name_option, const std::string& file_name);
bool matchModel(const std::string& model_option, const std::string& exif_model);

size_t searchJPEGFiles(const Options& options);
void handleDirectoryChange(const std::string& dir);

#endif // PARSER_H