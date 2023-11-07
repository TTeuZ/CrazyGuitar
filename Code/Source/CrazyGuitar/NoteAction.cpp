#include "NoteAction.h"

// Unreal Includes
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"
#include "Components/SphereComponent.h"

ANoteAction::ANoteAction() : chord(0) {
    this->PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> noteVisualAsset(TEXT("/Game/Shapes/Shape_Sphere.Shape_Sphere"));

    USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("NoteAction"));
    this->RootComponent = sphereComponent;

    FVector noteScale{FVector{0.2f, 1.f, 1.f} / 2.f};
    float sphereRadius = 50.f;

    sphereComponent->InitSphereRadius(sphereRadius);
    sphereComponent->SetCollisionProfileName(TEXT("Note"));
    sphereComponent->SetRelativeLocation(FVector{0.f, 0.f, 0.f});
    sphereComponent->SetRelativeScale3D(noteScale);

    if (noteVisualAsset.Succeeded()) {
        this->noteVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoteVisual"));
        this->noteVisual->SetStaticMesh(noteVisualAsset.Object);
        this->noteVisual->SetRelativeLocation(FVector{0, 0, -sphereRadius / 2});
        this->noteVisual->SetRelativeScale3D(noteScale * 0.8f);
        this->noteVisual->SetupAttachment(RootComponent);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find note material"));
    }
}

ANoteAction::ANoteAction(const uint8_t chord, const FVector position)
    : ANoteAction() {
    this->chord = chord;
    this->SetActorLocation(position);
}

uint8_t ANoteAction::getChord() const { return this->chord; }

FVector ANoteAction::getPosition() const { return this->GetActorLocation(); }

/*
*   Unreal Engine enforce us to use the name newChord instead of chord, otherwise
*   we received a compilation error
*/
void ANoteAction::setChord(const uint8_t newChord) { this->chord = newChord; }

void ANoteAction::setPosition(const FVector position) {
    this->SetActorLocation(position);
}

void ANoteAction::BeginPlay() {
    Super::BeginPlay();
    this->SetActorLocation(FVector{0, 0, 500});
}

void ANoteAction::Tick(const float deltaTime) {
    Super::Tick(deltaTime);
    this->move(deltaTime);
}

bool ANoteAction::isHit(const uint8_t chordHited, const int32_t positionHited) const {
    UE_LOG(LogTemp, Warning, TEXT("Chord: %d, Position: %d"), chordHited, positionHited);
    UE_LOG(LogTemp, Warning, TEXT("Hitbox start: %d, Hitbox end: %d"), HITBOX_START, HITBOX_END);

    return this->chord == chordHited && positionHited > -HITBOX_START && positionHited < -HITBOX_END;
}

void ANoteAction::move(const float deltaTime) {
    FVector location = GetActorLocation();
    location.Y -= deltaTime * 200;

    if (location.Y < -500) location.Y = 500;
    this->SetActorLocation(location);
}
