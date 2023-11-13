#include "ChartController.h"

// Unreal Includes
#include "Kismet/GameplayStatics.h"

AChartController::AChartController() : chart{nullptr} {};

void AChartController::SetupInputComponent() {
    Super::SetupInputComponent();
    this->EnableInput(this);

    this->InputComponent->BindAction("HitFirstChord", IE_Pressed, this, &AChartController::hitFirstChord);
    this->InputComponent->BindAction("HitSecondChord", IE_Pressed, this, &AChartController::hitSecondChord);
    this->InputComponent->BindAction("HitThirdChord", IE_Pressed, this, &AChartController::hitThirdChord);
    this->InputComponent->BindAction("HitFourthChord", IE_Pressed, this, &AChartController::hitFourthChord);

    this->InputComponent->BindAction("Start", IE_Released, this, &AChartController::startGame);
}

void AChartController::BeginPlay() { this->chart = static_cast<AChart*>(this->GetPawn()); }

void AChartController::startGame() { this->chart->startGame(); }

void AChartController::hitFirstChord() { this->chart->hitChord(0); }

void AChartController::hitSecondChord() { this->chart->hitChord(1); }

void AChartController::hitThirdChord() { this->chart->hitChord(2); }

void AChartController::hitFourthChord() { this->chart->hitChord(3); }