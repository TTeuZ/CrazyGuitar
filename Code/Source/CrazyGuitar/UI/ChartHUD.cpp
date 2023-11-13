#include "ChartHUD.h"

AChartHUD::AChartHUD() {}

void AChartHUD::DrawHUD() { Super::DrawHUD(); }

void AChartHUD::Tick(float deltaTime) { Super::Tick(deltaTime); }

void AChartHUD::BeginPlay() {
    Super::BeginPlay();

    if (this->PlayerSaveWidgetClass) {
        this->PlayerSaveWidget = CreateWidget<UPlayerSaveWidget>(this->GetWorld(), this->PlayerSaveWidgetClass);
        if (this->PlayerSaveWidget) {
            this->PlayerSaveWidget->AddToViewport();
            this->PlayerSaveWidget->updateDisplayedState(0, 0, 0);
        }
    }
}

void AChartHUD::updateDisplayedState(const float& score, const uint32& hits, const uint32& misses) {
    if (this->PlayerSaveWidget) this->PlayerSaveWidget->updateDisplayedState(score, hits, misses);
}