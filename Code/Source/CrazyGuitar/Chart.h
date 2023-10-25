// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cstdint>
#include <list>

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chart.generated.h"

constexpr uint8 MAX_CHORDS = 4;

constexpr uint8 HITBOX_START = 10;
constexpr uint8 HITBOX_END = 20;
constexpr uint8 HITBOX_SIZE = HITBOX_END - HITBOX_START;
constexpr uint8 HITBOX_CENTER = HITBOX_START + HITBOX_SIZE / 2;

UCLASS()
class CRAZYGUITAR_API AChart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChart();
    ~AChart();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void addNoteAction(void* noteAction);

    void removeNoteAction(void* noteAction);

    void clearNoteActions();

    void hitChord(uint8_t chord);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    std::list<void*> m_notes;
    int8_t noteSpeed;
};
