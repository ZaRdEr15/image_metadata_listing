#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "common_includes.h"
#include <algorithm> // std::transform, std::count, std::all_of

#define UTILITY_POS 0

enum OptionsResult {Success, InvalidOption, NoValue, MissingDirectory, TooManyArgs};

void showUsage(const std::string& utility);
void showArgs(int argc, char* argv[]);
void showOptions(const std::string& name, const std::string& date, const std::string& model);
OptionsResult getOptions(int argc, char* argv[], 
                std::string& name, std::string& date,
                std::string& model, std::string& directory_path);
void validateOptions(OptionsResult result, const std::string& utility);
inline void stringToLower(std::string& s) {
    std::transform(s.cbegin(), s.cend(), s.begin(), [](unsigned char c){ return std::tolower(c); });
}
bool matchDate(const std::string& date_option, const std::string& exif_date);
bool matchPattern(const std::string& text, const std::string& pattern);
bool matchName(std::string name_option, std::string file_name);
bool matchModel(std::string model_option, std::string exif_model);

#endif // ARGUMENTS_H