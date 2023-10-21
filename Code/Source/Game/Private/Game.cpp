#include "Game.hpp"

#include "Log.hpp"
#include "Modules/ModuleManager.h"

void FGame::StartupModule() {
    UE_LOG(LogGame, Log, TEXT("Game module starting up"));
}

void FGame::ShutdownModule() {
    UE_LOG(LogGame, Log, TEXT("Game module shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FGame, Game, "Game");