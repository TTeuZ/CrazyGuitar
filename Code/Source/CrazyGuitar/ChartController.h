#pragma once

// Personal Includes
#include "Chart.h"

// Unreal Includes
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// Must be the last include
#include "ChartController.generated.h"

UCLASS()
class CRAZYGUITAR_API AChartController : public APlayerController {
    GENERATED_BODY()

   public:
    AChartController();
    virtual ~AChartController() = default;

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
};
