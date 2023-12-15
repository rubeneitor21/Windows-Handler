#include "../rubLogger/rubLogger.h"
#include "windows.h"

#ifndef windowHandler_h
#define windowHandler_h

class windowHandler
{
public:
    windowHandler(RubLogger logger);

    void main();
    void update();
    void windowSize(RECT &lastWindow, int i, HWND handler, bool xSeparator, RECT &returnValue);
    bool checkChanges();

private:
    RubLogger logger;
};

#endif