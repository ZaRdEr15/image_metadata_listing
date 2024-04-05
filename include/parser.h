#include "common_includes.h"

#define COLUMN_WIDTH 25
#define FILENAME_WIDTH 50

enum DataType {CaptureDate, CameraModel};

inline std::string enumToString(DataType type);
inline std::string formatDate(std::string date);
void showData(std::string data, DataType type);

