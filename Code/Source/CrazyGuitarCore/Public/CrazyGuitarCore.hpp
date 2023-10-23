#ifndef CRAZY_GUITAR_CORE_HPP
#define CRAZY_GUITAR_CORE_HPP

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FCrazyGuitarCore : public IModuleInterface {
   public:
    static inline FCrazyGuitarCore& Get() {
        return FModuleManager::LoadModuleChecked<FCrazyGuitarCore>(
            "CrazyGuitarCore");
    }

    static inline bool IsAvailable() {
        return FModuleManager::Get().IsModuleLoaded("CrazyGuitarCore");
    }

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

#endif