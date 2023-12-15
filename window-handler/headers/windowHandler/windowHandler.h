#include "../rubLogger/rubLogger.h"

#ifndef windowHandler_h
#define windowHandler_h

class windowHandler
{
public:
    windowHandler(RubLogger logger);

    void main();
    void update();
    RECT windowSize(RECT lastWindow, RECT pos, HWND &handler, bool xSeparator);

private:
    RubLogger logger;
};

#endif