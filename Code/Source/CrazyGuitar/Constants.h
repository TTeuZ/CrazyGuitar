#pragma once

// Standard includes
#include <cstdint>

// Unreal includes
#include "CoreMinimal.h"

class Constants {
   public:
    Constants() = delete;
    virtual ~Constants() = default;

    // Chart constants
    constexpr static uint8_t MAX_CHORDS{4};
    const static FVector CHART_SIZE;
    const static FVector CHART_SCALE;
    const static FVector CHART_INITIAL_LOCATION;

    // Camera constants
    const static FVector CAMERA_INITIAL_LOCATION;

    // Notes constants
    const static float ZJUMP;
    const static FVector DEFAULT_NOTE_LOCATION;

    // Note constants
    constexpr static uint8_t HITBOX_SCALE{20};
    constexpr static uint8_t HITBOX_SIZE{44};
    constexpr static uint8_t HITBOX_START{154};
    constexpr static uint8_t HITBOX_END{HITBOX_START - HITBOX_SIZE};
    constexpr static uint8_t HITBOX_CENTER{HITBOX_START - (HITBOX_SIZE / 2)};
};