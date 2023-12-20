#include "../rubLogger/rubLogger.h"
#include "update.h"

Updater::Updater(RubLogger logger) : logger(logger) {}

bool Updater::update()
{
    INFO(logger, "Looking for updates...");

    int returnValue = system(".\\checkupdate.bat");

    if (returnValue == 0)
    {
        INFO(logger, "Up to date");
        return false;
    }
    else if (returnValue == 1)
    {
        INFO(logger, "Download finished\nClosing to update");
        return true;
    }

    return false;
}