#include "common_includes.h"

#define N_ARGUMENTS_MIN 2
#define N_ARGUMENTS_MAX 8

#define UTILITY_POS 0

void showUsage(std::string utility);
void showArgs(int argc, char* argv[]);
void showOptions(std::string name, std::string date, std::string model);
void getOptions(int argc, char* argv[], 
                std::string& name, std::string& date,
                std::string& model, std::string& directory_path);
