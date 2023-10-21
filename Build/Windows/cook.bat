@echo off

call "%~dp0vars.bat"

start "" "%UE4EDITOR_CMD_WIN64_EXE%" "%UPROJECT_PATH%" -run=cook -targetplatform=WindowsNoEditor