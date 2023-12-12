@echo off
echo Compilando...

g++ window-handler\main.cpp window-handler\headers\rubLogger\rubLogger.cpp window-handler\headers\windowHandler\windowHandler.cpp -o main.exe -mwindows

echo Listo
@REM start main.exe
