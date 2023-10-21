# ifeq ($(OS),Windows_NT)
# all:
# 	cat .\Build\instructions.md
# else 
# all:
# 	cat ./Build/instructions.md
# endif

build_editor_win64:
	.\Build\Windows\build.bat Development Editor

build_game_win64:
	.\Build\Windows\build.bat Development && .\Build\Windows\cook.bat

run_editor_win64:
	.\Build\Windows\editor.bat

run_game_win64:
	.\Build\Windows\game.bat