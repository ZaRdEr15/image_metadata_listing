#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "common_includes.h"

#define UTILITY_POS 0

enum OptionsResult {Success, InvalidOption, NoValue, MissingDirectory, TooManyArgs};

void showUsage(std::string_view utility);
void showArgs(int argc, char* argv[]);
void showOptions(const Options& options);
OptionsResult getOptions(int argc, char* argv[], Options& options);
void validateOptions(OptionsResult result, std::string_view utility);

#endif // ARGUMENTS_H