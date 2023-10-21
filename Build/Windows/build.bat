@echo off

call "%~dp0vars.bat"

call "%BUILD_BAT%" %PROJECT%%2 Win64 %1 "%UPROJECT_PATH%" -waitmutex -NoHotReload