#define N_ARGUMENTS 4

#define UTILITY_POS 0
#define FILENAME_POS 1
#define CAPTUREDATE_POS 2
#define CAMERAMODEL_POS 3

void showArgs(int argc, char* argv[]);
bool checkUsage(int argc, char *argv[]);