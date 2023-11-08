#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "NoteAction.generated.h"

UCLASS()
class CRAZYGUITAR_API ANoteAction : public AActor {
    GENERATED_BODY()

   public:
    ANoteAction();

    ANoteAction(uint8_t chord, const FVector position = FVector{0, 0, 0});

    virtual ~ANoteAction() = default;

    uint8_t getChord() const;

    const FVector getPosition() const;

    void setChord(uint8_t newChord);

    void setPosition(const FVector& position);

    virtual void Tick(float deltaTime) override;

    bool isHit(uint8_t chordHited, int32_t positionHited) const;

   protected:
    virtual void BeginPlay() override;

   private:
    constexpr static uint8_t HITBOX_SCALE{20};  // in percent
    constexpr static uint8_t HITBOX_START{154};
    constexpr static uint8_t HITBOX_SIZE{44};
    constexpr static uint8_t HITBOX_END{HITBOX_START - HITBOX_SIZE};
    constexpr static uint8_t HITBOX_CENTER{HITBOX_START - (HITBOX_SIZE / 2)};

    void move(float deltaTime);

    uint8_t chord;
    
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* noteVisual;
};
