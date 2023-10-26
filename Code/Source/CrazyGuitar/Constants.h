#pragma once

#include <cstdint>

class Constants {
   public:
    constexpr static uint8_t MAX_CHORDS = 4;

    constexpr static uint8_t HITBOX_START = 10;
    constexpr static uint8_t HITBOX_END = 20;
    constexpr static uint8_t HITBOX_SIZE = HITBOX_END - HITBOX_START;
    constexpr static uint8_t HITBOX_CENTER = HITBOX_START + HITBOX_SIZE / 2;
};