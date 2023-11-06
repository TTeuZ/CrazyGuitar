#include "NoteAction.h"

ANoteAction::ANoteAction() : chord(0) { PrimaryActorTick.bCanEverTick = true; }

ANoteAction::ANoteAction(uint8_t chord, float position) : ANoteAction() {
    this->chord = chord;
    SetActorLocation(FVector{position, 0, 0});
}

void ANoteAction::BeginPlay() { Super::BeginPlay(); }

void ANoteAction::Tick(float deltaTime) {
    Super::Tick(deltaTime);
    this->move(deltaTime);
}

bool ANoteAction::isHit(uint8_t chordHited, int32_t positionHited) {
    return chord == chordHited && positionHited > Constants::HITBOX_START &&
           positionHited < Constants::HITBOX_END;
}

void ANoteAction::move(float deltaTime) {
    FVector location = GetActorLocation();
    location.X += deltaTime * 100;
    SetActorLocation(location);
}
