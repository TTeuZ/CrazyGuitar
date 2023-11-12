#include "CrazyGuitarController.h"

// Unreal Includes
#include "Kismet/GameplayStatics.h"

ACrazyGuitarController::ACrazyGuitarController() : chart{nullptr}, playerSave{new PlayerSave{}} {};

void ACrazyGuitarController::SetupInputComponent() {
    Super::SetupInputComponent();
    this->EnableInput(this);

    this->InputComponent->BindAction("HitFirstChord", IE_Pressed, this, &ACrazyGuitarController::hitFirstChord);
    this->InputComponent->BindAction("HitSecondChord", IE_Pressed, this, &ACrazyGuitarController::hitSecondChord);
    this->InputComponent->BindAction("HitThirdChord", IE_Pressed, this, &ACrazyGuitarController::hitThirdChord);
    this->InputComponent->BindAction("HitFourthChord", IE_Pressed, this, &ACrazyGuitarController::hitFourthChord);

    this->InputComponent->BindAction("Start", IE_Released, this, &ACrazyGuitarController::startGame);
}

void ACrazyGuitarController::BeginPlay() { this->chart = static_cast<AChart*>(this->GetPawn()); }

void ACrazyGuitarController::startGame() { this->chart->startGame(); }

void ACrazyGuitarController::hitFirstChord() { this->chart->hitChord(0); }

void ACrazyGuitarController::hitSecondChord() { this->chart->hitChord(1); }

void ACrazyGuitarController::hitThirdChord() { this->chart->hitChord(2); }

void ACrazyGuitarController::hitFourthChord() { this->chart->hitChord(3); }