#include "Constants.h"

const FVector Constants::CHART_SIZE{10.f, 220.f, 60.f};
const FVector Constants::CHART_SCALE{CHART_SIZE / 50.f};
const FVector Constants::CHART_INITIAL_LOCATION{200.f, 0.f, 250.f};
const FVector Constants::CAMERA_INITIAL_LOCATION{-220.f, 0.f, -50.f};

const float Constants::ZJUMP{(CHART_SIZE.Z * 2) / (MAX_CHORDS + 1)};

const FVector Constants::DEFAULT_NOTE_LOCATION{CHART_INITIAL_LOCATION.X * 0.9f, 200.f,
                                               CHART_INITIAL_LOCATION.Z + CHART_SIZE.Z - ZJUMP};
