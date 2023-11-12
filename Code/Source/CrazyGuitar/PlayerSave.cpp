#include "PlayerSave.h"

PlayerSave::PlayerSave() : score{0}, hits{0}, misses{0} {}

uint64_t PlayerSave::getScore() const { return this->score; }

uint32_t PlayerSave::getHits() const { return this->hits; }

uint32_t PlayerSave::getMisses() const { return this->misses; }

void PlayerSave::updateScore(const uint32_t adder) { this->score += adder; }

void PlayerSave::updateHits(const uint32_t adder) { this->hits += adder; }

void PlayerSave::updateMisses(const uint32_t adder) { this->misses += adder; }

void PlayerSave::cleanUpStatus() {
    this->score = 0;
    this->hits = 0;
    this->misses = 0;
}