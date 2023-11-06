#include "AtorFlutuante.h"

AAtorFlutuante::AAtorFlutuante()
{
    PrimaryActorTick.bCanEverTick = true;
    this->tempoExecucao = 0.f;
    this->alturaDelta = 0.f;
    this->novaLocalizacao = FVector(0.f, 0.f, 0.f);
}

void AAtorFlutuante::BeginPlay()
{
    Super::BeginPlay();
}

void AAtorFlutuante::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    this->novaLocalizacao = this->GetActorLocation();
    // UE_LOG(LogTemp, Warning, TEXT("novaLocalizacao: %s\n"), *this->novaLocalizacao.ToString());
    this->alturaDelta = FMath::Sin(this->tempoExecucao + DeltaTime) - FMath::Sin(this->tempoExecucao);
    // UE_LOG(LogTemp, Warning, TEXT("alturaDelta: %f\n"), this->alturaDelta);
    this->novaLocalizacao.Z += this->alturaDelta * 30.f;
    // UE_LOG(LogTemp, Warning, TEXT("novaLocalizacao: %s\n"), *this->novaLocalizacao.ToString());
    this->tempoExecucao += DeltaTime;
    // UE_LOG(LogTemp, Warning, TEXT("tempoExecucao: %f\n"), this->tempoExecucao);
    this->SetActorLocation(this->novaLocalizacao);
}
