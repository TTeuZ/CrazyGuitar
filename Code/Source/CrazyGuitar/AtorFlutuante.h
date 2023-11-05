#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AtorFlutuante.generated.h"

UCLASS()
class CRAZYGUITAR_API AAtorFlutuante : public AActor
{
    GENERATED_BODY()
    
public:
    AAtorFlutuante();
    virtual ~AAtorFlutuante() = default;

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    float tempoExecucao;
    float alturaDelta;
    FVector novaLocalizacao;
};
