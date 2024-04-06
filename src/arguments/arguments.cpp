#include <unistd.h>     // getopt, optarg, optind
#include <regex>
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

/* 
    Check exact match of the capture date and return
    true if matches, otherwise false
*/
bool matchDate(std::string date_option, std::string exif_date) {
    // If the option is empty return true
    bool match = true;
    if(!date_option.empty()) { // if there is a capture date option, then check if there is a match
        match = false;
        // Exact date match
        if(date_option == exif_date) {
            match = true;
        }
    }
    return match;
}

// Remove a wildcard to allow to create regex pattern
inline std::string removeWildcard(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '*'), s.end());
    return s;
}

/* 
    Construct a regex match pattern with a provided regex char set and a possible
    end matching "$" symbol (applicable to model) based on the wildcard symbol
    provided in the option
    Would be an exact match if no wildcard symbol provided
*/
std::string getMatchPattern(std::string opt, std::string regex, std::string end_match = "") {
    if(countWildcard(opt)) { // if there is a wildcard symbol
        if(opt[0] == '*') {
            if(opt.size() == 1) {
                return regex + "+"; // "*" as option
            }
            return regex + "*" + removeWildcard(opt) + end_match; // "*abc" as option
        }
        return "^" + removeWildcard(opt) + regex + "*"; // "abc*" as option
    }
    return "^" + opt + end_match; // exact match as there is no wildcard symbols
}

/* 
    Make a regex match with an option and compare to provided file name
    If the file name matches, returns true, otherwise false;
    If the option is empty, returns true;
*/
bool matchName(std::string name_option, std::string file_name) {
    // If the option is empty return true
    bool match = true;
    if(!name_option.empty()) { // if there is a name option, then check if there is a match
        match = false;
        // Set regex according to wildcard symbol
        std::string regex_pattern = getMatchPattern(name_option, "[\\w-]"); // matches a-zA-Z0-9_-
        regex_pattern += "\\.jpg$"; // matches for any .jpg file

        std::regex name_regex(regex_pattern);
        
        stringToLower(file_name); // case insensitive match
        match = std::regex_match(file_name, name_regex);
    }
    return match;
}

/* 
    Make a regex match with an option and compare to provided camera model
    If the model matches, returns true, otherwise false;
    If the option is empty, returns true;
*/
bool matchModel(std::string model_option, std::string exif_model) {
    // If the option is empty return true
    bool match = true;
    if(!model_option.empty()) { // if there is a camera model option, then check if there is a match
        match = false;
        // Set regex according to wildcard symbol
        std::string regex_pattern = getMatchPattern(model_option, "[\\w -]", "$"); // matches a-zA-Z0-9_- and a whitespace

        std::regex model_regex(regex_pattern);
        
        stringToLower(exif_model); // case insensitive match
        match = std::regex_match(exif_model, model_regex);
    }
    return match;
}