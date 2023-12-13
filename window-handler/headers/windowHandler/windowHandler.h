#include "../rubLogger/rubLogger.h"

#ifndef windowHandler_h
#define windowHandler_h

class windowHandler
{
public:
    windowHandler(RubLogger logger);

    void main();
    void update();

private:
    RubLogger logger;
};

#endif