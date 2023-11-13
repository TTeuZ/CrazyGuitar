#include "PlayerSaveWidget.h"

UPlayerSaveWidget::UPlayerSaveWidget(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer} {}

void UPlayerSaveWidget::NativeConstruct() {
    Super::NativeConstruct();
    this->scoreTxt->SetVisibility(ESlateVisibility::Visible);
    this->hitsTxt->SetVisibility(ESlateVisibility::Visible);
    this->missesTxt->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerSaveWidget::updateDisplayedState(const float& score, const uint32_t& hits, const uint32_t& misses) {
    if (this->scoreTxt) this->scoreTxt->SetText(FText::FromString("Score: " + FString::SanitizeFloat(score)));
    if (this->hitsTxt) this->hitsTxt->SetText(FText::FromString("Hits: " + FString::FromInt(hits)));
    if (this->missesTxt) this->missesTxt->SetText(FText::FromString("Misses: " + FString::FromInt(misses)));
}