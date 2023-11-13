#include "ChartHUD.h"

AChartHUD::AChartHUD() {}

void AChartHUD::DrawHUD() { Super::DrawHUD(); }

void AChartHUD::Tick(float deltaTime) { Super::Tick(deltaTime); }

void AChartHUD::BeginPlay() {
    Super::BeginPlay();

    if (this->playerSaveWidgetClass) {
        this->playerSaveWidget = CreateWidget<UPlayerSaveWidget>(this->GetWorld(), this->playerSaveWidgetClass);
        if (this->playerSaveWidget) {
            this->playerSaveWidget->AddToViewport();
            this->playerSaveWidget->updateDisplayedState(0, 0, 0);
            this->playerSaveWidget->updateDisplayedStreak(0);
        }
    }
}

void AChartHUD::updateDisplayedState(const float& score, const uint32& hits, const uint32& misses) {
    if (this->playerSaveWidget) this->playerSaveWidget->updateDisplayedState(score, hits, misses);
}

void AChartHUD::updateDisplayedStreak(const uint32& streak) {
    if (this->playerSaveWidget) this->playerSaveWidget->updateDisplayedStreak(streak);
}