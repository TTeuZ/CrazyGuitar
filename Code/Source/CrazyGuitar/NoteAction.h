// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteAction.generated.h"

UCLASS()
class CRAZYGUITAR_API ANoteAction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteAction();
    ANoteAction(uint8_t chord, float position = 0);

    ~ANoteAction() = default;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    bool isHit(uint8_t chordHited, int32_t positionHited);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    void move(float deltaTime);

    uint8_t chord;
};
