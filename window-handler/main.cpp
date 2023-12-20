#include "headers/rubLogger/rubLogger.h"
#include "headers/windowHandler/windowHandler.h"
#include "headers/updater/update.h"

int main()
{
    RubLogger logger;
    windowHandler handler = windowHandler(logger);
    Updater updater = Updater(logger);

    bool updateAvailable = updater.update();

    if (updateAvailable)
    {
        system("start powershell .\\update.ps1");
        return 0;
    }
    else
    {
        handler.main();
    }

    return 0;
}