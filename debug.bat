@echo off

echo Compilando...
g++ window-handler\main.cpp window-handler\headers\rubLogger\rubLogger.cpp window-handler\headers\windowHandler\windowHandler.cpp -o main.exe -lgdi32

if %errorlevel% equ 0 (
    echo Lanzando...
    .\main.exe
) else (
    @REM echo Fallo en compilacion.
)
