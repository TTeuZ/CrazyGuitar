@echo off

set ROOTDIR=%~dp0
set ROOTDIR=%ROOTDIR:~0,-15%

set PROJECT=CrazyGuitar
set PROJECT_DIR=%ROOTDIR%\Code
set UPROJECT_PATH=%PROJECT_DIR%\%PROJECT%.uproject

@REM Put here the path for your Unreal Engine
set UE4_DIR=D:\7. Dev\Unreal\UE_4.27
set BUILD_BAT=%UE4_DIR%\Engine\Build\BatchFiles\Build.bat

set UE4EDITOR_WIN64_EXE=%UE4_DIR%\Engine\Binaries\Win64\UE4editor.exe
set UE4EDITOR_CMD_WIN64_EXE=%UE4_DIR%\Engine\Binaries\Win64\UE4editor-cmd.exe

set GAME_EXE=%PROJECT_DIR%\Binaries\Win64\%PROJECT%.exe