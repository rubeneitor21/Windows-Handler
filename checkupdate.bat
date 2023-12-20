@echo off

.\PortableGit\bin\git.exe pull > temp.txt

set /p "variable=" <temp.txt

if /i "%variable%" equ "Already up to date." (
    del temp.txt
    exit /b 0
)
else (
    del temp.txt
    exit /b 1
)