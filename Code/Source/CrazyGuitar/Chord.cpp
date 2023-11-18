#include "Chord.h"

#include "Chart.h"

const FVector AChord::CHORD_BASE_POSITION{
    FVector{-10.0f, -AChart::CHART_SIZE.Y * 0.85f, AChart::CHART_SIZE.Z* AChart::CHORDS_SPACE_PERCENT_IN_CHART}};
const FVector AChord::CHORD_INITIAL_LOCATION{AChart::CHART_LOCATION + AChord::CHORD_BASE_POSITION};
const FString AChord::CHORD_MATERIAL_PATH{TEXT("/Game/StarterContent/Materials/M_Metal_Burnished_Steel")};
const FString AChord::CHORD_MESH_PATH{TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder")};
const FVector AChord::CHORD_SCALE{FVector{0.01f, 0.01f, AChart::CHART_SCALE.Y * AChart::CHORDS_SPACE_PERCENT_IN_CHART}};
const FVector AChord::CHORD_SIZE{AChord::CHORD_SCALE * 50.f};

static const float CHORD_POS_JUMP{AChart::CHART_SIZE.Z * 2 / AChart::MAX_CHORDS};

AChord::AChord() : index{0}, notes{nullptr}, position{0.f}, hitbox{nullptr}, material{nullptr}, visual{nullptr} {
    this->PrimaryActorTick.bCanEverTick = false;

    this->SetRootComponent(this->CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

    this->material = LoadObject<UMaterial>(nullptr, *AChord::CHORD_MATERIAL_PATH);
    this->visual = LoadObject<UStaticMesh>(nullptr, *AChord::CHORD_MESH_PATH);

    if (!this->material) {
        this->material = UMaterial::GetDefaultMaterial(MD_Surface);
        UE_LOG(LogTemp, Warning, TEXT("AChord::AChord: Failed to load material"));
    }

    if (!this->visual) {
        UE_LOG(LogTemp, Warning, TEXT("AChord::AChord: Failed to load mesh"));
        UE_LOG(LogTemp, Error, TEXT("AChord::AChord: Unable to create visual"));
    } else {
        this->SetActorScale3D(FVector{0.02f, 0.02f, AChart::CHART_SCALE.Y});
        this->chordVisual = this->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChordVisual"));
        this->chordVisual->SetStaticMesh(this->visual);
        this->chordVisual->SetMaterial(0, this->material);
        this->chordVisual->SetRelativeLocation(FVector{0.f, 0.f, 0.f});
        this->chordVisual->SetRelativeRotation(FRotator{0.f, 0.f, 90.0f});
        this->chordVisual->SetupAttachment(this->GetRootComponent());
    }

    this->SetActorLocation(AChord::CHORD_BASE_POSITION - FVector{0.f, 0.f, this->position});

    // Log actor location
    UE_LOG(LogTemp, Log, TEXT("AChord::AChord: Actor location: %s"), *this->GetActorLocation().ToString());
    UE_LOG(LogTemp, Log, TEXT("AChord::AChord: Chord created"));
}

void AChord::BeginPlay() { 
    Super::BeginPlay(); 
    this->createHitbox();
}

int AChord::getIndex() const { return this->index; }

float AChord::getPosition() const { return this->position; }

void AChord::setIndex(const uint8_t newIndex) {
    this->index = newIndex;
    this->position = CHORD_POS_JUMP * this->index;

    FVector newLocation{AChord::CHORD_BASE_POSITION - FVector{0.f, 0.f, this->position}};
    UE_LOG(LogTemp, Log, TEXT("AChord::setIndex: New location: %s"), *newLocation.ToString());
    this->SetActorLocation(newLocation);
}

void AChord::createHitbox() {
    this->hitbox = GetWorld()->SpawnActor<AHitbox>(AHitbox::StaticClass());
    this->hitbox->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}
