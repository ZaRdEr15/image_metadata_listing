#include "common_includes.h"

#define N_ARGUMENTS_MIN 2
#define N_ARGUMENTS_MAX 8

#define UTILITY_POS 0

enum OptionsResult {Success, InvalidOption, MissingDirectory, TooManyArgs};

void showUsage(std::string utility);
void showArgs(int argc, char* argv[]);
void showOptions(std::string name, std::string date, std::string model);
void validateOptions(int argc, char* argv[], 
                     std::string& name, std::string& date,
                     std::string& model, std::string& directory_path);
