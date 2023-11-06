#pragma once

#include "Constants.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "NoteAction.generated.h"

UCLASS()
class CRAZYGUITAR_API ANoteAction : public AActor {
    GENERATED_BODY()

   public:
    ANoteAction();
    ANoteAction(const uint8_t chord, const FVector position = FVector{0, 0, 0});

    ~ANoteAction() = default;

    uint8_t getChord() const;
    void setChord(const uint8_t newChord);

    FVector getPosition() const;
    void setPosition(const FVector position);

    virtual void Tick(const float deltaTime) override;

    bool isHit(const uint8_t chordHited, const int32_t positionHited) const;

   protected:
    virtual void BeginPlay() override;

   private:
    void move(const float deltaTime);

    uint8_t chord;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* noteVisual;
};
