#include "Chart.h"

AChart::AChart() {
    PrimaryActorTick.bCanEverTick = true;

    this->noteSpeed = 1;
    this->noteActions = std::list<ANoteAction*>();
}

AChart::~AChart() { this->clearNoteActions(); }

void AChart::BeginPlay() { Super::BeginPlay(); }

void AChart::Tick(float deltaTime) { Super::Tick(deltaTime); }

void AChart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChart::addNoteAction(ANoteAction* noteAction) {
    noteActions.push_back(noteAction);
}

void AChart::removeNoteAction(ANoteAction* noteAction) {
    noteActions.remove(noteAction);
}

void AChart::clearNoteActions() {
    //  this->noteActions.clear();
}
