#include <unistd.h>     // getopt, optarg, optind
#include "arguments.h"

// Show utility usage
void showUsage(const std::string& utility) {
    std::cout << "\nUsage: " << utility 
              << " [-n \"file_name\"]" 
              << " [-d \"capture_date\"]" 
              << " [-m \"camera_model\"]" 
              << " directory\n"
              << "\n\"file_name\" and \"camera_model\" support wildcard symbol '*' for partial matching:\n"
              << "\"abcde\" is matched by: \"abc*\", \"*cde\", \"*\", \"*c*\", \"ab*e\".\n"
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
void showOptions(const std::string& name, const std::string& date, const std::string& model) {
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
                std::cerr << "ERROR: The option provided is invalid." << std::endl;
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

/*
    Validates if options are correct and saves each option to the
    corresponding string variable
*/
void validateOptions(OptionsResult result, const std::string& utility) {
    switch(result) {
        case NoValue:
            std::cerr << "ERROR: No value provided for the option." << std::endl;
        case InvalidOption:
        case MissingDirectory:
        case TooManyArgs:
            showUsage(utility);
        case Success:
            break;
    }
}

/* 
    Return true if: 
    exact match of the capture date
    date option is empty
*/
bool matchDate(const std::string& date_option, const std::string& exif_date) {
    if(date_option.empty()) { 
        return true; 
    }
    return (date_option == exif_date);
}

/*
    Checks if there is a match between the text and the pattern
    Pattern can contain a '*' wildcard symbol
    It matches any character sequence or an empty sequence
    Returns true if the text matches the pattern, returns false if it does not
*/
bool matchPattern(const std::string& text, const std::string& pattern) {
    size_t i = 0; // Text index
    size_t j = 0; // Pattern index
    size_t textBacktrack = -1; // Position to reset to if mismatch of characters or end of pattern
    size_t nextToWildcard = -1; // Position to reset to if mismatch of characters
    bool wildcardFound = false;
    while(i < text.size()) {
        if(j < pattern.size() && text[i] == pattern[j]) {
            i++;
            j++;
        } else if(j < pattern.size() && pattern[j] == '*') {
            wildcardFound = true;
            j++;
            nextToWildcard = j;
            textBacktrack = i; // Save and go back from the next to it
        } else if(wildcardFound == false){
            return false; // Characters arent same and no wildcard so no match
        } else {
            // Wildcard was present, reset to next to wildcard and continue text
            j = nextToWildcard;
            textBacktrack++; // Go next from the saved position
            i = textBacktrack;
        }
    }
    // Check if all characters left are wildcard characters
    // if no there is no match
    // If the string is at the end returns true
    return std::all_of(pattern.begin() + j, pattern.end(), [](char c) { return c == '*'; });
}

/*
    Match any .jpg file depending on the wildcard symbol
    If the file name matches, returns true, otherwise false;
    If the option is empty, returns true for any .jpg file;
*/
bool matchName(std::string name_option, std::string file_name) {
    if(!name_option.empty()) { // if there is a name option, then check if there is a match
        stringToLower(file_name); // case insensitive match
        name_option += ".jpg";
        return matchPattern(file_name, name_option);
    }
    // match any .jpg file
    return matchPattern(file_name, "*.jpg");
}

/* 
    Match any camera model depending on the wildcard symbol
    If the model matches, returns true, otherwise false;
    If the option is empty, returns true;
*/
bool matchModel(std::string model_option, std::string exif_model) {
    if(!model_option.empty()) { // if there is a camera model option, then check if there is a match
        stringToLower(exif_model); // case insensitive match
        return matchPattern(exif_model, model_option);
    }
    // If the option is empty return true
    return true;
}