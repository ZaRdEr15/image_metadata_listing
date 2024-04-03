#include <iostream>
#include "TinyEXIF.h"

#define N_ARGUMENTS 4

#define UTILITY_POS 0
#define FILENAME_POS 1
#define CAPTUREDATE_POS 2
#define CAMERAMODEL_POS 3

void showUsage(std::string utility) {
    std::cout << "Usage: " << "[" << utility << "] file_name capture_date camera_model" << std::endl;
    std::cout << "file_name:    case-insensitive, exact match or partial match with * wildcard symbol" << std::endl;
    std::cout << "capture_date: exact match" << std::endl;
    std::cout << "camera_model: case-insensitive, exact match or partial match with * wildcard symbol" << std::endl;
}

bool checkArgc(int argc, char *argv[]) {
    if(argc != N_ARGUMENTS) {
        std::cout << "Error: there were " << argc - 1 << " arguments while the utility requires " << N_ARGUMENTS - 1 << ".\n";
        showUsage(argv[UTILITY_POS]);
        return false;
    }
    return true;
}

bool checkArgv(char *argv[]) {
    // Check if 1, 2 and 3 arguments are correct
    return true;
}

bool checkUsage(int argc, char *argv[]) {
    if(!checkArgc(argc, argv)) {
        return false;
    }
    if(!checkArgv(argv)) {
        return false;
    }
}

int main(int argc, char *argv[]) {
    if(!checkUsage(argc, argv)) {
        return 1;
    }

    return 0;
}