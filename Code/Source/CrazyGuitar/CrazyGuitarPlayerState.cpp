#include "CrazyGuitarPlayerState.h"

ACrazyGuitarPlayerState::ACrazyGuitarPlayerState() : hits{0}, misses{0} {}

uint32_t ACrazyGuitarPlayerState::getHits() const { return this->hits; }

uint32_t ACrazyGuitarPlayerState::getMisses() const { return this->misses; }

void ACrazyGuitarPlayerState::computeHit(const float adder) {
    this->SetScore(this->GetScore() + adder);
    ++this->hits;
}
void ACrazyGuitarPlayerState::computeMiss() { ++this->misses; }

void ACrazyGuitarPlayerState::cleanUpStatus() {
    this->SetScore(0);
    this->hits = 0;
    this->misses = 0;
}