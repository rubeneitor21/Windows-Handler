#ifndef rubLogger_H
#define rubLogger_H

#include <iostream>
#include <string.h>
class RubLogger
{
public:
    RubLogger();

    void loggerInfo(std::string text, const char file[], int line);
};

#define INFO(logger, text) logger.loggerInfo(text, __FILE__, __LINE__)
#endif