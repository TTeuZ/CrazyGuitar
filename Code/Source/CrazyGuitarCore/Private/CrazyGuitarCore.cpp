#include "CrazyGuitarCore.hpp"

#include "Log.hpp"
#include "Modules/ModuleManager.h"

void FCrazyGuitarCore::StartupModule() {
    UE_LOG(LogCrazyGuitarCore, Log, TEXT("CrazyGuitarCore module starting up"));
}

void FCrazyGuitarCore::ShutdownModule() {
    UE_LOG(LogCrazyGuitarCore, Log,
           TEXT("CrazyGuitarCore module shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FCrazyGuitarCore, CrazyGuitarCore,
                              "CrazyGuitarCore");