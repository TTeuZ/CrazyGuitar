#pragma once

// Personal Includes
#include "Chart.h"
#include "PlayerSave.h"

// Unreal Includes
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// Must be the last include
#include "CrazyGuitarController.generated.h"

UCLASS()
class CRAZYGUITAR_API ACrazyGuitarController : public APlayerController {
    GENERATED_BODY()

   public:
    ACrazyGuitarController();
    virtual ~ACrazyGuitarController() = default;

    virtual void SetupInputComponent() override;

   protected:
    virtual void BeginPlay() override;

   private:
    void startGame();

    void hitFirstChord();
    void hitSecondChord();
    void hitThirdChord();
    void hitFourthChord();

    AChart* chart;
    PlayerSave* playerSave;
};
