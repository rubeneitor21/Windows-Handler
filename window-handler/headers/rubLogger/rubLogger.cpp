#include "rubLogger.h"

RubLogger::RubLogger() {}

void RubLogger::loggerInfo(const char text[], const char file[], int line)
{
    std::cout << "\033[36m[INFO]\033[0m: " << file << " - " << line << ": " << text << std::endl;
}