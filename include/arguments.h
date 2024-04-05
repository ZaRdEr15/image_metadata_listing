#include "common_includes.h"

#define N_ARGUMENTS_MIN 2
#define N_ARGUMENTS_MAX 8

#define WILDCARD_MAX 1

#define UTILITY_POS 0

enum OptionsResult {Success, InvalidOption, NoValue, MissingDirectory, TooManyArgs};

void showUsage(std::string utility);
void showArgs(int argc, char* argv[]);
void showOptions(std::string name, std::string date, std::string model);
int countWildcard(std::string s);
void validateOptions(int argc, char* argv[], 
                     std::string& name, std::string& date,
                     std::string& model, std::string& directory_path);
             
// Convert all characters in a string to lowercase
inline void stringToLower(std::string& s) {
    for(size_t i = 0; i < s.size(); i++) {
        s[i] = static_cast<char>(tolower(s[i]));
    }
}
