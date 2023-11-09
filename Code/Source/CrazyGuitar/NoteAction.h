#pragma once

// Personal includes
#include "Constants.h"

// Unreal Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"

// Must be the last include
#include "NoteAction.generated.h"

UCLASS()
class CRAZYGUITAR_API ANoteAction : public AActor {
    GENERATED_BODY()

   public:
    ANoteAction();

    virtual ~ANoteAction() = default;

    uint8_t getChord() const;
    const FVector getPosition() const;

    void setChord(const uint8_t newChord);
    void setCanMove(const bool newCanMove);
    void setPosition(const FVector& position);

    virtual void Tick(float deltaTime) override;

    bool isHit(const uint8_t& chordHited, const int32_t& positionHited) const;

    void playNote();

   protected:
    virtual void BeginPlay() override;

   private:
    void move(const float& deltaTime);

    uint8_t chord;
    bool canMove;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* noteVisual;
};
