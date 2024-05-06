#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "common_includes.h"
#include <algorithm> // std::transform, std::count, std::all_of

#define UTILITY_POS 0

enum OptionsResult {Success, InvalidOption, NoValue, MissingDirectory, TooManyArgs};

void showUsage(std::string_view utility);
void showArgs(int argc, char* argv[]);
void showOptions(std::string_view name, std::string_view date, std::string_view model);
OptionsResult getOptions(int argc, char* argv[], 
                std::string& name, std::string& date,
                std::string& model, std::string& directory_path);
void validateOptions(OptionsResult result, std::string_view utility);

#endif // ARGUMENTS_H