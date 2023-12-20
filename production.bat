@echo off
echo Compilando...

g++ window-handler\main.cpp window-handler\headers\rubLogger\rubLogger.cpp window-handler\headers\windowHandler\windowHandler.cpp window-handler\headers\updater\update.cpp -o RubBs.exe -mwindows

echo Listo
@REM start main.exe
