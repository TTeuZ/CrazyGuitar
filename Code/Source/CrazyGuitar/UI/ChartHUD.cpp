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
            this->playerSaveWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if (this->preStartWidgetClass) {
        this->preStartWidget = CreateWidget<UPreStartWidget>(this->GetWorld(), this->preStartWidgetClass);
        if (this->preStartWidget) {
            this->preStartWidget->AddToViewport();
            this->preStartWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void AChartHUD::displayGameStats() {
    if (this->preStartWidget) this->preStartWidget->SetVisibility(ESlateVisibility::Hidden);
    if (this->playerSaveWidget) this->playerSaveWidget->SetVisibility(ESlateVisibility::Visible);
}

void AChartHUD::updateDisplayedState(const float score, const uint32 hits, const uint32 misses) {
    if (this->playerSaveWidget) this->playerSaveWidget->updateDisplayedState(score, hits, misses);
}

void AChartHUD::updateDisplayedStreak(const uint32 streak) {
    if (this->playerSaveWidget) this->playerSaveWidget->updateDisplayedStreak(streak);
}