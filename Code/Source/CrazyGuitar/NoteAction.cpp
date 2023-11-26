#include "NoteAction.h"

// Personal Includes
#include "Chart.h"
#include "Notes.h"
#include "PlayerSave.h"

// Unreal Includes
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"
#include "Components/SphereComponent.h"

const std::array<FLinearColor, 4> ANoteAction::CHORD_COLORS{
    FLinearColor{0.3f, 1.0f, 0.3f}, 
    FLinearColor{1.0f, 0.3f, 0.3f},
    FLinearColor{0.3f, 0.3f, 1.0f}, 
    FLinearColor{1.0f, 1.0f, 0.3f}};

ANoteAction::ANoteAction() : chord{0}, canMove{false}, notes{nullptr}, visual{nullptr} {
    this->PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> visualAsset{TEXT("/Game/Shapes/Shape_Sphere.Shape_Sphere")};

    USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("NoteAction"));
    this->RootComponent = sphereComponent;

    FVector noteScale{FVector{1.f, 1.f, 1.f} * ANoteAction::SCALE};
    float sphereRadius = 50.f * ANoteAction::SCALE;

    sphereComponent->InitSphereRadius(sphereRadius);
    sphereComponent->SetCollisionProfileName(TEXT("Note"));
    sphereComponent->SetRelativeLocation(FVector{0.f, 0.f, 0.f});
    sphereComponent->SetRelativeScale3D(noteScale);

    if (visualAsset.Succeeded()) {
        this->visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoteVisual"));
        this->visual->SetStaticMesh(visualAsset.Object);
        this->visual->SetRelativeLocation(FVector{0, 0, -sphereRadius});
        this->visual->SetRelativeScale3D(noteScale);
        this->visual->SetCachedMaxDrawDistance(1500.f);
        this->visual->SetCollisionProfileName(TEXT("NoCollision"));
        this->visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        this->visual->SetCastShadow(false);
        this->visual->SetupAttachment(this->RootComponent);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find note material"));
    }
}

uint8_t ANoteAction::getChord() const { return this->chord; }

const FVector ANoteAction::getPosition() const { return this->GetActorLocation(); }

void ANoteAction::setChord(const uint8_t newChord) { 
    this->chord = newChord; 

    // Muda a cor da nota de acordo com o acorde
    // 1. Obtem o material da nota
    UMaterialInterface* originalMaterial{this->visual->GetMaterial(0)};
    UMaterialInstanceDynamic* material{UMaterialInstanceDynamic::Create(originalMaterial, this)};
    // 2. Obtem a cor do acorde
    FLinearColor chordColor{ANoteAction::CHORD_COLORS[this->chord]};
    // 3. Altera a cor do material
    material->SetVectorParameterValue(TEXT("Color"), chordColor);
    // 4. Atualiza o material
    this->visual->SetMaterial(0, material);
}

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
    location.X -= deltaTime * (this->notes->getGameBPM());
    this->setPosition(location);

    if (location.X < -AChart::CHART_SIZE.Y) {
        APlayerSave* playerState{
            static_cast<APlayerSave*>(this->GetWorld()->GetFirstPlayerController()->GetPawn()->GetPlayerState())};

        if (playerState) playerState->computeMiss();
        this->notes->removeNote(this);
    }
}
