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

    uint32_t getHits() const;
    uint32_t getMisses() const;

    void computeHit(const float adder);
    void computeMiss();

    void cleanUpStatus();

   private:
    uint32_t hits;
    uint32_t misses;

    void updateHUDContent();
};
