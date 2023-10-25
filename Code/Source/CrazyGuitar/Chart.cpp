// Fill out your copyright notice in the Description page of Project Settings.


#include "Chart.h"


// Sets default values
AChart::AChart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    this->noteSpeed = 1;
    this->m_notes = std::list<void*>();
}

AChart::~AChart()
{
    this->clearNoteActions();
}

// Called when the game starts or when spawned
void AChart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChart::addNoteAction(void* noteAction)
{
    m_notes.push_back(noteAction);
}

void AChart::removeNoteAction(void* noteAction)
{
    m_notes.remove(noteAction);
}

void AChart::clearNoteActions()
{
    m_notes.clear();
}
