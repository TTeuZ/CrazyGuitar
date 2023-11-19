#include "NoteAction.h"

// Personal Includes
#include "Chart.h"
#include "Notes.h"

// Unreal Includes
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"
#include "Components/SphereComponent.h"

static constexpr float SCALE{0.5f};

ANoteAction::ANoteAction() : chord{0}, canMove{false}, notes{nullptr} {
    this->PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> noteVisualAsset{
        TEXT("/Game/Shapes/Shape_Sphere.Shape_Sphere")};

    USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("NoteAction"));
    this->RootComponent = sphereComponent;

    FVector noteScale{FVector{1.f, 1.f, 1.f} * SCALE};
    float sphereRadius = 50.f * SCALE;

    sphereComponent->InitSphereRadius(sphereRadius);
    sphereComponent->SetCollisionProfileName(TEXT("Note"));
    sphereComponent->SetRelativeLocation(FVector{0.f, 0.f, 0.f});
    sphereComponent->SetRelativeScale3D(noteScale);

    if (noteVisualAsset.Succeeded()) {
        this->noteVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoteVisual"));
        this->noteVisual->SetStaticMesh(noteVisualAsset.Object);
        this->noteVisual->SetRelativeLocation(FVector{0, 0, -sphereRadius});
        this->noteVisual->SetRelativeScale3D(noteScale);
        this->noteVisual->SetCollisionProfileName(TEXT("NoCollision"));
        this->noteVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        this->noteVisual->SetupAttachment(this->RootComponent);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find note material"));
    }
}

uint8_t ANoteAction::getChord() const { return this->chord; }

const FVector ANoteAction::getPosition() const { return this->GetActorLocation(); }

void ANoteAction::setChord(const uint8_t newChord) { this->chord = newChord; }

void ANoteAction::setCanMove(const bool newCanMove) { this->canMove = newCanMove; }

void ANoteAction::setNotes(Notes* const newNotes) {
    newNotes->noteActions.push_back(this);
    this->notes = newNotes;
}

void ANoteAction::setPosition(const FVector& position) { this->SetActorLocation(position); }

void ANoteAction::Tick(float deltaTime) {
    Super::Tick(deltaTime);
    this->move(deltaTime);
}

void ANoteAction::hit() { this->notes->removeNote(this); }

void ANoteAction::playNote() { UE_LOG(LogTemp, Warning, TEXT("Play note")); }

void ANoteAction::BeginPlay() { Super::BeginPlay(); }

void ANoteAction::move(const float& deltaTime) {
    if (!this->canMove) return;

    FVector location{this->getPosition()};
    location.X -= deltaTime * 400;
    this->setPosition(location);

    if (location.X < -AChart::CHART_SIZE.Y) this->notes->removeNote(this);
}
