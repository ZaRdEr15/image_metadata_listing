#include "arguments.h"
#include <iostream>
#include <vector>

// Show utility usage
void showUsage(std::string utility) {
    std::cout << "Usage: " << "[" << utility << "] [\"file_name\"] [\"capture_date\"] [\"camera_model\"]" << "\n"
              << "file_name:    case-insensitive, exact match or partial match with * wildcard symbol" << "\n"
              << "capture_date: exact match" << "\n"
              << "camera_model: case-insensitive, exact match or partial match with * wildcard symbol" << std::endl;
}

// Show arguments provided for the utility
void showArgs(int argc, char* argv[]) {
    std::vector<std::string> string_args(argv + 1, argv + argc);
    std::cout << "Arguments: " << std::endl;
    for(const std::string& arg : string_args) {
        std::cout << arg << "\n";
    }
}

// Check if the amount of arguments corresponds to the required amount 
bool checkArgc(int argc, char* argv[]) {
    if(argc != N_ARGUMENTS) {
        std::cout << "Error: there were " << argc - 1 << " arguments while the utility requires " << N_ARGUMENTS - 1 << ".\n";
        showUsage(argv[UTILITY_POS]);
        return false;
    }
    return true;
}

// Check if arguments are correct
bool checkArgv(char* argv[]) {
    // Check if 1, 2 and 3 arguments are correct, show correct entering
    return true;
}

// Check if amount of arguments and arguments are correct
bool checkUsage(int argc, char *argv[]) {
    if(!checkArgc(argc, argv)) {
        return false;
    }
    if(!checkArgv(argv)) {
        return false;
    }
    return true;
}