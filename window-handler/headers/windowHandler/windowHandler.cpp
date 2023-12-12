#include "windowHandler.h"
#include "../rubLogger/rubLogger.h"

windowHandler::windowHandler(RubLogger logger) : logger(logger)
{
    logger = logger;
}

void windowHandler::main()
{
    INFO(logger, "Starting service...");
    INFO(logger, "Started");
}