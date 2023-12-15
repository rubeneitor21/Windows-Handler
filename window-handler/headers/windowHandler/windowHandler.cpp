#include "windowHandler.h"
#include "../rubLogger/rubLogger.h"
#include <windows.h>
#include <map>
#include <string.h>
#include <thread>

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

void windowHandler::windowSize(RECT &lastWindow, int i, HWND handler, bool xSeparator, RECT &returnValue)
{
    // WARN: restructurar esto que va a tener fallitos seguro
    if (!xSeparator)
    {
        returnValue.left = lastWindow.right / 2 + lastWindow.left / (2 * i) * 8.2;
        returnValue.right = lastWindow.right / 2;

        returnValue.top = lastWindow.top;
        returnValue.bottom = lastWindow.bottom;
    }
    else
    {
        returnValue.left = lastWindow.left;
        returnValue.right = lastWindow.right;

        returnValue.top = lastWindow.bottom / 2 + lastWindow.top / (2 * i);
        returnValue.bottom = lastWindow.bottom / 2;
    }

    // RECT newLast;
    // newLast.left = lastWindow.left;
    // newLast.right = lastWindow.right / 2;

    // // Cambiar esto si funciona luego
    // newLast.top = lastWindow.top;
    // newLast.bottom = lastWindow.bottom;

    if (handler != NULL)
    {
        ShowWindow(handler, SW_NORMAL);
        auto r = SetWindowPos(handler, 0, lastWindow.left, lastWindow.top, lastWindow.right / (!xSeparator ? 2 : 1), lastWindow.bottom / (xSeparator ? 2 : 1), 0);
        lastWindow.left = returnValue.left;
        lastWindow.right = returnValue.right;
        lastWindow.top = returnValue.top;
        lastWindow.bottom = returnValue.bottom;
    }
}

std::map<HWND, std::string> control = {};
bool windowHandler::checkChanges()
{
    if (control.size() != ventanas.size())
    {
        control = ventanas;
        return true;
    }
    for (auto &pair : ventanas)
    {
        auto it = control.find(pair.first);
        if (it == control.end())
        {
            return true;
        }
    }
    return false;
    control = ventanas;
}

void windowHandler::update()
{
    while (true)
    {
        ventanas = {};
        HDESK desktop = GetThreadDesktop(GetCurrentThreadId());
        bool completado = EnumDesktopWindows(desktop, windows, 0);

        if (checkChanges())
        {

            int ventanasI = 1;
            int maxVentanas = ventanas.size();

            INFO(logger, std::to_string(maxVentanas));

            int width = (desktopSize.right / dpiX * 96);
            int height = desktopSize.bottom + (desktopSize.bottom / dpiY) - 10;

            RECT lastWindow;
            lastWindow.right = width;
            lastWindow.bottom = height;
            lastWindow.left = 0;
            lastWindow.top = 0;
            HWND lastHandle = NULL;

            for (auto &pair : ventanas)
            {
                if (ventanasI > 4)
                {
                    ShowWindow(pair.first, SW_MINIMIZE);
                    continue;
                }
                RECT nuevaVentana;
                INFO(logger, std::to_string(lastWindow.left) + " " + std::to_string(lastWindow.top) + " " + std::to_string(lastWindow.right) + " " + std::to_string(lastWindow.bottom) + " ");

                windowSize(lastWindow, ventanasI, lastHandle, ventanasI % 2 != 0, nuevaVentana);

                INFO(logger, std::to_string(nuevaVentana.left) + " " + std::to_string(nuevaVentana.top) + " " + std::to_string(nuevaVentana.right) + " " + std::to_string(nuevaVentana.bottom) + " ");

                ShowWindow(pair.first, SW_NORMAL);
                SetWindowPos(pair.first, 0, nuevaVentana.left, nuevaVentana.top, nuevaVentana.right, nuevaVentana.bottom, 0);
                lastHandle = pair.first;
                ventanasI++;
            }
        }
    }
}

// if (maxVentanas == 2)
//         {
//             ShowWindow(pair.first, SW_NORMAL);
//             auto r = SetWindowPos(pair.first, 0, width / 2 * ventanasI, 0, width / 2, height, 0);
//             ventanasI++;
//         }
//         else if (maxVentanas > 2)
//         {
//             if (ventanasI >= 4)
//             {
//                 ShowWindow(pair.first, SW_MINIMIZE);
//             }
//             else
//             {
//                 int posX = ventanasI % 2 == 0 ? 0 : width / 2;
//                 int posY = ventanasI < 2 ? 0 : height / 2;
//                 ShowWindow(pair.first, SW_NORMAL);
//                 auto r = SetWindowPos(pair.first, 0, posX, posY, width / 2, height / 2, 0);
//                 ventanasI++;
//             }
//         }