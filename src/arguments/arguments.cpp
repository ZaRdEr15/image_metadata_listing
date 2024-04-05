#include <unistd.h>     // getopt, optarg, optind
#include <algorithm>    // std::count
#include "arguments.h"

// Show utility usage
void showUsage(std::string utility) {
    std::cout << "\nUsage: " << utility 
              << " [-n \"file_name\"]" 
              << " [-d \"capture_date\"]" 
              << " [-m \"camera_model\"]" 
              << " directory\n"
              << "\n\"file_name\" and \"camera_model\" support wildcard symbol '*' for partial matching, such as:\n"
              << "Starting with: \"abc*\", ending with: \"*abc\", or any: \"*\".\n"
              << "\"capture_date\" must be an exact match such as \"2024-01-01\".\n" 
              << "Every optional field must be inside quotation marks: -n \"file\", -m \"model\" -d \"2010-01-01\".\n" << std::endl;
    exit(EXIT_FAILURE);
}

// Show arguments provided for the utility
void showArgs(int argc, char* argv[]) {
    std::cout << "Arguments: " << std::endl;
    for(int i = 0; i < argc; i++) {
        std::cout << "[" << i << "]: " << argv[i] << "\n";
    }
}

// Show options provided for the utility
void showOptions(std::string name, std::string date, std::string model) {
    std::cout << "Options: \n";
    if(!name.empty()) {
        std::cout << "File name: " << name << "\n";
    }
    if(!date.empty()) {
        std::cout << "Capture date: " << date << "\n";
    }
    if(!model.empty()) {
        std::cout << "Camera model: " << model << "\n";
    }      
}

/*
    Saves options to corresponding string variables and
    returns a result code
*/
OptionsResult getOptions(int argc, char* argv[], 
                std::string& name, std::string& date,
                std::string& model, std::string& directory_path) {
    int opt;
    // Check if option that was provided has a value
    bool name_opt = false; 
    bool date_opt = false;
    bool model_opt = false;
    // Handles options with values such as -n, -d and -m 
    while((opt = getopt(argc, argv, "n:d:m:")) != EOF) {
        switch(opt) {
            case 'n':
                name = optarg;
                name_opt = true;
                break;
            case 'd':
                date = optarg;
                date_opt = true;
                break;
            case 'm':
                model = optarg;
                model_opt = true;
                break;
            default:
                return InvalidOption;
        }
    }

    // Return NoValue result if option provided but without a value
    if(name_opt) {
        if(name.empty()) return NoValue;
    }
    if(date_opt) {
        if(date.empty()) return NoValue;
    }
    if(model_opt) {
        if(model.empty()) return NoValue;
    }

    if(optind >= argc) {
        std::cerr << "ERROR: No directory provided." << std::endl;
        return MissingDirectory;
    }
    if(optind != (argc - 1)) {
        std::cerr << "ERROR: Too many arguments provided." << std::endl;
        return TooManyArgs;
    }
    directory_path = argv[optind];
    return Success;
}

// Count how many wildcard symbols inside string
int countWildcard(std::string s) {
    return std::count(s.begin(), s.end(), '*');
}

/*
    Validates if options are correct and saves each option to the
    corresponding string variable
*/
void validateOptions(int argc, char* argv[], 
                     std::string& name, std::string& date,
                     std::string& model, std::string& directory_path) {
    switch(getOptions(argc, argv, name, date, model, directory_path)) {
        case InvalidOption:
        case NoValue:
            std::cerr << "ERROR: No value provided for the option." << std::endl;
        case MissingDirectory:
        case TooManyArgs:
            showUsage(argv[UTILITY_POS]);
        case Success:
            break;
    }

    // Validate model and name to have only one '*' wildcard symbol
    if(countWildcard(name) > WILDCARD_MAX || countWildcard(model) > WILDCARD_MAX) {
        std::cerr << "ERROR: There was more than 1 '*' symbol for option." << std::endl;
        showUsage(argv[UTILITY_POS]);
    }
}