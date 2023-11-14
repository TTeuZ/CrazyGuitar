#include "PreStartWidget.h"

UPreStartWidget::UPreStartWidget(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer} {}

void UPreStartWidget::NativeConstruct() {
    Super::NativeConstruct();
    this->titleTxt->SetVisibility(ESlateVisibility::Visible);
    this->infoTxt->SetVisibility(ESlateVisibility::Visible);

    this->PlayAnimation(this->fadeInOut, 0.f, 0, EUMGSequencePlayMode::Forward, 1.f);
}