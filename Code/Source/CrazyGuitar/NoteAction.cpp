// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteAction.h"


// Sets default values
ANoteAction::ANoteAction() : chord(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

ANoteAction::ANoteAction(uint8_t chord, float position) : ANoteAction()
{
    this->chord = chord;
    SetActorLocation(FVector{position, 0, 0});
}

// Called when the game starts or when spawned
void ANoteAction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANoteAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    this->move(DeltaTime);
}

void ANoteAction::move(float deltaTime)
{
    FVector location = GetActorLocation();
    location.X += deltaTime * 100;
    SetActorLocation(location);
}
