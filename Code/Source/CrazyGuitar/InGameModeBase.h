#pragma once

// Unreal includes
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Must be the last include
#include "InGameModeBase.generated.h"

UCLASS()
class CRAZYGUITAR_API AInGameModeBase : public AGameModeBase {
    GENERATED_BODY()

   public:
    AInGameModeBase();
};
