#include "windowHandler.h"
#include "../rubLogger/rubLogger.h"
#include <windows.h>
#include <map>
#include <string.h>

std::map<HWND, std::string> ventanas = {};
RECT desktopSize;
UINT flags = SWP_NOSIZE | SWP_NOZORDER;

UINT dpiX;
UINT dpiY;

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

    HDC hdc = GetDC(0);                    // Obtener el contexto del dispositivo para el sistema
    dpiX = GetDeviceCaps(hdc, LOGPIXELSX); // Obtener la escala DPI en el eje X
    dpiY = GetDeviceCaps(hdc, LOGPIXELSY); // Obtener la escala DPI en el eje X
    ReleaseDC(0, hdc);

    SystemParametersInfoA(SPI_GETWORKAREA, 0, &desktopSize, 0);

    if (completado)
    {
        INFO(logger, std::to_string(dpiX) + " " + std::to_string(dpiY) + " " + std::to_string(desktopSize.right / dpiX * 96) + " " + std::to_string(desktopSize.bottom / dpiY * 96));
        for (auto &pair : ventanas)
        {
            INFO(logger, pair.second);
        }
        update();
        INFO(logger, "Started\n");
    }
}

void windowHandler::update()
{
    int ventanasI = 0;
    int maxVentanas = ventanas.size();

    INFO(logger, std::to_string(maxVentanas));

    int width = (desktopSize.right / dpiX * 96);
    int height = desktopSize.bottom + (desktopSize.bottom / dpiY) - 10;

    for (auto &pair : ventanas)
    {
        if (maxVentanas == 2)
        {
            ShowWindow(pair.first, SW_NORMAL);
            auto r = SetWindowPos(pair.first, 0, width / 2 * ventanasI, 0, width / 2, height, 0);
            ventanasI++;
        }
        else if (maxVentanas > 2)
        {
            if (ventanasI >= 4)
            {
                ShowWindow(pair.first, SW_MINIMIZE);
            }
            else
            {
                int posX = ventanasI % 2 == 0 ? 0 : width / 2;
                int posY = ventanasI < 2 ? 0 : height / 2;
                ShowWindow(pair.first, SW_NORMAL);
                auto r = SetWindowPos(pair.first, 0, posX, posY, width / 2, height / 2, 0);
                ventanasI++;
            }
        }
        // INFO(logger, std::to_string(r));
    }
}