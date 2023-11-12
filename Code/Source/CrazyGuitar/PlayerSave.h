#pragma once

// Unreal includes
#include "CoreMinimal.h"

class CRAZYGUITAR_API PlayerSave {
   public:
    PlayerSave();
    virtual ~PlayerSave() = default;

    uint64_t getScore() const;
    uint32_t getHits() const;
    uint32_t getMisses() const;

    void updateScore(const uint32_t adder);
    void updateHits(const uint32_t adder);
    void updateMisses(const uint32_t adder);

    void cleanUpStatus();

   private:
    uint64_t score;
    uint32_t hits;
    uint32_t misses;
};