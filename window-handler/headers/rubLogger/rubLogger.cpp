#include "rubLogger.h"
#include <string.h>
#include <chrono>
#include <ctime>

RubLogger::RubLogger() {}

void RubLogger::loggerInfo(std::string text, const char file[], int line)
{
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // Convertir a una estructura de tiempo local
    std::tm *localTime = std::localtime(&currentTime);
    std::cout << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec
              << " \033[36m[INFO]\033[0m: " << file << " - " << line << ": " << text << std::endl;
}