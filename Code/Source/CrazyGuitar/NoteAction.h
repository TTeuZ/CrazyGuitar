#pragma once

// Unreal Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"

// Must be the last include
#include "NoteAction.generated.h"

class Notes;

UCLASS()
class CRAZYGUITAR_API ANoteAction : public AActor {
    GENERATED_BODY()

   public:
    ANoteAction();
    virtual ~ANoteAction() = default;

    uint8_t getChord() const;
    const FVector getPosition() const;

    /*
     *   Unreal Engine enforce us to use the name newChord instead of chord, otherwise
     *   we received a compilation error
     */
    void setChord(const uint8_t newChord);
    void setCanMove(const bool newCanMove);
    void setNotes(Notes* const newNotes);
    void setPosition(const FVector& position);

    virtual void Tick(float deltaTime) override;

    bool isHit(const uint8_t& chordHited, const int32_t& positionHited) const;

    void playNote();

   protected:
    virtual void BeginPlay() override;

   private:
    constexpr static uint8_t HITBOX_SCALE{20};
    constexpr static uint8_t HITBOX_SIZE{44};
    constexpr static uint8_t HITBOX_START{154};
    constexpr static uint8_t HITBOX_END{HITBOX_START - HITBOX_SIZE};
    constexpr static uint8_t HITBOX_CENTER{HITBOX_START - (HITBOX_SIZE / 2)};

    void move(const float& deltaTime);

    uint8_t chord;
    bool canMove;
    Notes* notes;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* noteVisual;
};
