@echo off

echo Compilando...
g++ window-handler\main.cpp window-handler\headers\rubLogger\rubLogger.cpp window-handler\headers\windowHandler\windowHandler.cpp -o main.exe

if %errorlevel% equ 0 (
    echo Lanzando...
    .\main.exe
) else (
    echo Fallo en compilacion.
)
