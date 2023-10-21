#ifndef GAME_HPP
#define GAME_HPP

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FGame : public IModuleInterface {
   public:
    static inline FGame& Get() {
        return FModuleManager::LoadModuleChecked<FGame>("Game");
    }

    static inline bool IsAvailable() {
        return FModuleManager::Get().IsModuleLoaded("Game");
    }

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

#endif