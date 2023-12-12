#ifndef rubLogger_H
#define rubLogger_H

#include <iostream>
class RubLogger
{
public:
    RubLogger();

    void loggerInfo(const char text[], const char file[], int line);
};

#define INFO(logger, text) logger.loggerInfo(text, __FILE__, __LINE__)
#endif