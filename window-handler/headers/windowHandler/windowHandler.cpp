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
    if (IsWindowVisible(hwnd) && MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY) != nullptr)
    {
        TCHAR windowTitle[256];
        if (GetWindowText(hwnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0])) > 0)
        {

            WINDOWPLACEMENT placement;
            placement.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(hwnd, &placement);

            if (placement.showCmd != SW_HIDE && GetWindowLong(hwnd, GWL_STYLE) > 0)
            {
                if ((GetWindowLong(hwnd, GWL_STYLE) & WS_POPUPWINDOW) &&
                    !(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) &&
                    !(placement.showCmd != SW_SHOWMAXIMIZED && GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_APPWINDOW))
                    // TODO: Just a couple of exeptions more to handle alt + space and some menus correctly
                    ventanas.insert(std::make_pair(hwnd, (std::string)windowTitle));
            }
        }
    }
    return true;
}

//* This actually looks good
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

        std::thread update_thread(windowHandler::update, this);
        update_thread.join();
        INFO(logger, "Started\n");
    }
}

// TODO: A little rework would be nice to. Better order of params
void windowHandler::windowSize(RECT &lastWindow, int i, HWND handler, bool xSeparator, RECT &returnValue)
{
    if (!xSeparator)
    {
        returnValue.left = lastWindow.right / 2 + lastWindow.left / (2 * i) * 8.2; // This feels wrong
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

    //? Should change this?
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

//* Works great, but should return the diff hwnd for the refresh
//* windows goes like crazy when updating position
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

// TODO: This needs to be re-done to add conditional reloading, just for not reloading the whole thing
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

            //? Something might be off
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
                INFO(logger, "LastWindow: " + std::to_string(lastWindow.left) + " " + std::to_string(lastWindow.top) + " " + std::to_string(lastWindow.right) + " " + std::to_string(lastWindow.bottom) + " ");

                windowSize(lastWindow, ventanasI, lastHandle, ventanasI % 2 != 0, nuevaVentana);

                INFO(logger, "NewWindow " + std::to_string(nuevaVentana.left) + " " + std::to_string(nuevaVentana.top) + " " + std::to_string(nuevaVentana.right) + " " + std::to_string(nuevaVentana.bottom) + " ");

                ShowWindow(pair.first, SW_NORMAL);
                SetWindowPos(pair.first, 0, nuevaVentana.left, nuevaVentana.top, nuevaVentana.right, nuevaVentana.bottom, 0);
                lastHandle = pair.first;
                ventanasI++;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}