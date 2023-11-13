#include "PlayerSave.h"

APlayerSave::APlayerSave() : hits{0}, misses{0} {}

uint32_t APlayerSave::getHits() const { return this->hits; }

uint32_t APlayerSave::getMisses() const { return this->misses; }

void APlayerSave::computeHit(const float adder) {
    this->SetScore(this->GetScore() + adder);
    ++this->hits;
}
void APlayerSave::computeMiss() { ++this->misses; }

void APlayerSave::cleanUpStatus() {
    this->SetScore(0);
    this->hits = 0;
    this->misses = 0;
}