@echo off

call "%~dp0vars.bat"

start "" "%UE4EDITOR_WIN64_EXE%" "%UPROJECT_PATH%" %*