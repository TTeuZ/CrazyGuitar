#pragma once

#include "Constants.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteAction.generated.h"

UCLASS()
class CRAZYGUITAR_API ANoteAction : public AActor {
    GENERATED_BODY()

   public:
    ANoteAction();
    ANoteAction(uint8_t chord, float position = 0);

    ~ANoteAction() = default;

    virtual void Tick(float deltaTime) override;

    bool isHit(uint8_t chordHited, int32_t positionHited);

   protected:
    virtual void BeginPlay() override;

   private:
    void move(float deltaTime);

    uint8_t chord;
};
