#pragma once

#include <cstdint>

class Constants {
   public:
    constexpr static uint8_t HITBOX_SCALE = 20; // in percent
    constexpr static uint8_t HITBOX_START = 154;
    constexpr static uint8_t HITBOX_SIZE = 44;
    constexpr static uint8_t HITBOX_END = HITBOX_START-HITBOX_SIZE;
    constexpr static uint8_t HITBOX_CENTER = HITBOX_START-(HITBOX_SIZE/2);
};
