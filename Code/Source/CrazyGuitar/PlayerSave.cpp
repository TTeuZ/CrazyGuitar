#include "PlayerSave.h"

// Personal Includes
#include "UI/ChartHUD.h"

APlayerSave::APlayerSave() : hits{0}, misses{0} {}

uint32_t APlayerSave::getHits() const { return this->hits; }

uint32_t APlayerSave::getMisses() const { return this->misses; }

void APlayerSave::computeHit(const float adder) {
    this->SetScore(this->GetScore() + adder);
    ++this->hits;
    this->updateHUDContent();
}

void APlayerSave::computeMiss() {
    ++this->misses;
    this->updateHUDContent();
}

void APlayerSave::cleanUpStatus() {
    this->SetScore(0);
    this->hits = 0;
    this->misses = 0;
    this->updateHUDContent();
}

void APlayerSave::updateHUDContent() {
    AChartHUD* HUD{static_cast<AChartHUD*>(this->GetWorld()->GetFirstPlayerController()->GetHUD())};
    if (HUD) HUD->updateDisplayedState(this->GetScore(), this->hits, this->misses);
}