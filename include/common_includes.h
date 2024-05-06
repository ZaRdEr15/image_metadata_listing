#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H

#include <iostream>
#include <string>

struct Options {
    std::string name, date, model, path;
    bool name_set = false;
    bool date_set = false;
    bool model_set = false;
};

#endif // COMMON_INCLUDES_H