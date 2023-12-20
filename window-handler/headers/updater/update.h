#ifndef updater_H
#define updater_H

#include <iostream>
#include "../rubLogger/rubLogger.h"
class Updater
{
public:
    Updater(RubLogger logger);

    bool update();

private:
    RubLogger logger;
};

#endif