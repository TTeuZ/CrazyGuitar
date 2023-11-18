#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

// Must be the last include
#include "PlayerSave.generated.h"

UCLASS()
class CRAZYGUITAR_API APlayerSave : public APlayerState {
    GENERATED_BODY()

   public:
    constexpr static float HIT_SCORE{100.f};

    APlayerSave();
    virtual ~APlayerSave() = default;

    void computeHit(const float adder);
    void computeMiss();

    void cleanUpStatus();

   private:
    void updateHUDContent();

    uint32_t hits;
    uint32_t misses;
    uint32_t hitStreak;
};
