#include "rubLogger.h"
#include <string.h>

RubLogger::RubLogger() {}

void RubLogger::loggerInfo(std::string text, const char file[], int line)
{
    std::cout << "\033[36m[INFO]\033[0m: " << file << " - " << line << ": " << text << std::endl;
}