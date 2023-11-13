#include "PlayerSave.h"

// Personal Includes
#include "UI/ChartHUD.h"

APlayerSave::APlayerSave() : hits{0}, misses{0}, hitStreak{0} {}

uint32_t APlayerSave::getHits() const { return this->hits; }

uint32_t APlayerSave::getMisses() const { return this->misses; }

void APlayerSave::computeHit(const float adder) {
    this->SetScore(this->GetScore() + adder);
    ++this->hits;
    ++this->hitStreak;
    this->updateHUDContent();
}

void APlayerSave::computeMiss() {
    ++this->misses;
    this->hitStreak = 0;
    this->updateHUDContent();
}

void APlayerSave::cleanUpStatus() {
    this->SetScore(0);
    this->hits = 0;
    this->misses = 0;
    this->updateHUDContent();
}

void APlayerSave::updateHUDContent() {
    // This Way to access the HUD inplies the access to the PlayerController. This can be made because the player
    // controller is instantiated at the world.
    AChartHUD* HUD{static_cast<AChartHUD*>(this->GetWorld()->GetFirstPlayerController()->GetHUD())};
    if (HUD) {
        HUD->updateDisplayedState(this->GetScore(), this->hits, this->misses);
        HUD->updateDisplayedStreak(this->hitStreak);
    }
}