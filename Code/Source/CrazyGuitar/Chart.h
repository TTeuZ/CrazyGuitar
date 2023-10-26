#pragma once

#include <cstdint>
#include <list>

#include "Constants.h"
#include "NoteAction.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Chart.generated.h"

UCLASS()
class CRAZYGUITAR_API AChart : public APawn {
    GENERATED_BODY()

   public:
    AChart();
    ~AChart();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    void addNoteAction(ANoteAction* noteAction);

    void removeNoteAction(ANoteAction* noteAction);

    void hitChord(uint8_t chord);

   protected:
    virtual void BeginPlay() override;

   private:
    void clearNoteActions();

    std::list<ANoteAction*> noteActions;
    int8_t noteSpeed;
};
