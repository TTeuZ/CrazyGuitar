#pragma once

// Personal Includes
#include "PlayerSaveWidget.h"

// Unreal Includes
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"

// Must be the last include
#include "ChartHUD.generated.h"

UCLASS()
class CRAZYGUITAR_API AChartHUD : public AHUD {
    GENERATED_BODY()

   public:
    AChartHUD();
    virtual ~AChartHUD() = default;

    virtual void DrawHUD() override;

    virtual void Tick(float deltaTime) override;

    virtual void BeginPlay() override;

    // For some reason, the compiler doesn't allow us to use cstdint classes only here.
    UFUNCTION()
    void updateDisplayedState(const float& score, const uint32& hits, const uint32& misses);

    UFUNCTION()
    void updateDisplayedStreak(const uint32& streak);

   private:
    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UUserWidget> playerSaveWidgetClass;

    UPlayerSaveWidget* playerSaveWidget;
};
