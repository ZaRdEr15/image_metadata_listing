#include <unistd.h> // getopt, optarg, optind
#include "arguments.h"

// Show utility usage
void showUsage(std::string utility) {
    std::cout << "Usage: " << utility 
              << " [-n \"file_name\"] [-d \"capture_date\"] [-m \"camera_model\"] directory" << std::endl;
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
    std::cout << "Options: \n"
              << "File name: " << name
              << "\nCapture date: " << date
              << "\nCamera model: " << model << std::endl;
}

void getOptions(int argc, char* argv[], 
                std::string& name, std::string& date,
                std::string& model, std::string& directory_path) {
    int opt;
    // Handles options with values such as -n, -d and -m 
    while((opt = getopt(argc, argv, "n:d:m:")) != EOF) {
        switch(opt) {
            case 'n':
                name = optarg;
                break;
            case 'd':
                date = optarg;
                break;
            case 'm':
                model = optarg;
                break;
            default:
                showUsage(argv[UTILITY_POS]);
        }
    }
    if(optind >= argc) {
        std::cerr << "ERROR: No directory provided." << std::endl;
        showUsage(argv[UTILITY_POS]);
    }
    if(optind != (argc - 1)) {
        std::cerr << "ERROR: Too many arguments provided." << std::endl;
        showUsage(argv[UTILITY_POS]);
    }
    directory_path = argv[optind];
}