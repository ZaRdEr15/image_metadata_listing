#include <unistd.h>     // getopt, optarg, optind
#include "arguments.h"

void showUsage(std::string_view utility) {
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

void showArgs(int argc, char* argv[]) {
    std::cout << "Arguments: " << std::endl;
    for(int i = 0; i < argc; i++) {
        std::cout << "[" << i << "]: " << argv[i] << "\n";
    }
}

void showOptions(const Options& options) {
    std::cout << "Options: \n";
    if(!options.name.empty()) {
        std::cout << "File name: " << options.name << "\n";
    }
    if(!options.date.empty()) {
        std::cout << "Capture date: " << options.date << "\n";
    }
    if(!options.model.empty()) {
        std::cout << "Camera model: " << options.model << "\n";
    }      
}

inline bool noOptSet(bool opt_set, std::string_view s){
    return (opt_set && s.empty());
}

OptionsResult getOptions(int argc, char* argv[], Options& options) {
    int opt;
    // Handles options with values such as -n, -d and -m 
    while((opt = getopt(argc, argv, "n:d:m:")) != EOF) {
        switch(opt) {
            case 'n':
                options.name = optarg;
                options.name_set = true;
                break;
            case 'd':
                options.date = optarg;
                options.date_set = true;
                break;
            case 'm':
                options.model = optarg;
                options.model_set = true;
                break;
            default:
                std::cerr << "ERROR: The option provided is invalid." << std::endl;
                return InvalidOption;
        }
    }

    if(noOptSet(options.name_set, options.name) || 
       noOptSet(options.date_set, options.date) || 
       noOptSet(options.model_set, options.model)) {
        std::cerr << "ERROR: No value provided for the option." << std::endl;
        return NoValue;
    }

    if(optind >= argc) {
        std::cerr << "ERROR: No directory provided." << std::endl;
        return MissingDirectory;
    }
    if(optind != (argc - 1)) {
        std::cerr << "ERROR: Too many arguments provided." << std::endl;
        return TooManyArgs;
    }
    options.path = argv[optind];
    return Success;
}

void validateOptions(OptionsResult result, std::string_view utility) {
    switch(result) {
        case NoValue:
        case InvalidOption:
        case MissingDirectory:
        case TooManyArgs:
            showUsage(utility);
        case Success:
            break;
    }
}