@echo off

echo Compilando...
g++ window-handler\main.cpp window-handler\headers\rubLogger\rubLogger.cpp window-handler\headers\windowHandler\windowHandler.cpp -o main.exe

echo Lanzando...
start main.exe
