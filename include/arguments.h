#include <iostream>
#include <vector>
#include <unistd.h>

using std::cout;
using std::cerr;
using std::endl;

#define N_ARGUMENTS_MIN 2
#define N_ARGUMENTS_MAX 8

#define UTILITY_POS 0

void showUsage(std::string utility);
void showArgs(int argc, char* argv[]);
std::vector<std::string> getOptions(int argc, char* argv[], std::string& directory_path);
void showOptions(std::vector<std::string> options);