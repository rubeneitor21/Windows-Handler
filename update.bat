@echo off
echo Esperando cierre...
timeout /t 5 /nobreak > nul

echo Compilando

g++ window-handler\main.cpp window-handler\headers\rubLogger\rubLogger.cpp window-handler\headers\windowHandler\windowHandler.cpp window-handler\headers\updater\update.cpp -o RubBs.exe -mwindows

set /p respuesta=Quieres añadir el programa al startup de windows? (Y/n): 

if /i "%respuesta%"=="y" (
    copy ".\RubBs.exe" "C:\Users\%username%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup"
) else if /i "%respuesta%"=="n" (
    echo ":("
) else (
    copy ".\RubBs.exe C:\Users\%username%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup"
)

start .\RubBs.exe

exit /b