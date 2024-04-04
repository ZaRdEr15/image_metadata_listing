#include "arguments.h"

// Show utility usage
void showUsage(std::string utility) {
    cout << "Usage: " << utility << " [-n \"file_name\"] [-d \"capture_date\"] [-m \"camera_model\"] directory" << endl;
    exit(EXIT_FAILURE);
}

// Show arguments provided for the utility
void showArgs(int argc, char* argv[]) {
    std::vector<std::string> string_args(argv, argv + argc); // convert char*[] to vector<string>
    cout << "Arguments: " << endl;
    for(const std::string& arg : string_args) {
        cout << arg << "\n";
    }
}

// Show provided options
void showOptions(std::vector<std::string> options) {
    cout << "Options: " << endl;
    for(const auto& opt : options) {
        cout << opt << endl;
    }
}

std::vector<std::string> getOptions(int argc, char* argv[], std::string& directory_path) {
    int opt;
    std::vector<std::string> opt_strings;
    // Handles options with values such as -n, -d and -m 
    while((opt = getopt(argc, argv, "n:d:m:")) != EOF) {
        switch(opt) {
            case 'n':
            case 'd':
            case 'm':
                opt_strings.push_back(optarg);
                break;
            default:
                showUsage(argv[UTILITY_POS]);
        }
    }
    if(optind >= argc) {
        cerr << "ERROR: No directory provided." << endl;
        showUsage(argv[UTILITY_POS]);
    }
    if(optind != (argc - 1)) {
        cerr << "ERROR: Extra arguments provided." << endl;
        showUsage(argv[UTILITY_POS]);
    }
    directory_path = argv[optind];
    return opt_strings;
}