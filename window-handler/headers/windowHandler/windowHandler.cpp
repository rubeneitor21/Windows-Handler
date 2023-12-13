#include "windowHandler.h"
#include "../rubLogger/rubLogger.h"
#include <windows.h>
#include <map>
#include <string.h>

std::map<HWND, std::string> ventanas = {};

windowHandler::windowHandler(RubLogger logger) : logger(logger) {}

BOOL CALLBACK windows(HWND hwnd, LPARAM lparam)
{

    if (IsWindowVisible(hwnd))
    {
        TCHAR windowTitle[256];
        if (GetWindowText(hwnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0])) > 0)
        {
            WINDOWPLACEMENT placement;
            placement.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(hwnd, &placement);
            if (placement.showCmd != SW_HIDE && GetWindowLong(hwnd, GWL_STYLE) > 0)
            {
                // std::cout << windowTitle << " " << placement.showCmd << " " << GetWindowLong(hwnd, GWL_STYLE) << std::endl;
                ventanas.insert(std::make_pair(hwnd, (std::string)windowTitle));
            }
        }
    }
    return true;
}

void windowHandler::main()
{
    INFO(logger, "Starting service...");

    HDESK desktop = GetThreadDesktop(GetCurrentThreadId());

    bool completado = EnumDesktopWindows(desktop, windows, 0);

    if (completado)
    {
        for (auto &pair : ventanas)
        {
            // std::string a = "hola";
            INFO(logger, pair.second);
        }
        INFO(logger, "Started\n");
    }
}