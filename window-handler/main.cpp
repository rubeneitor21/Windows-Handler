#include "headers/rubLogger/rubLogger.h"
#include "headers/windowHandler/windowHandler.h"

int main()
{
    RubLogger logger;
    windowHandler handler = windowHandler(logger);

    handler.main();

    return 0;
}