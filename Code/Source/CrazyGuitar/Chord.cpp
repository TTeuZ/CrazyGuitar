#include "Chord.h"

// Personal includes
#include "Chart.h"

const FVector AChord::CHORD_BASE_POSITION{
    FVector{-10.0f, -AChart::CHART_SIZE.Y * 0.85f, AChart::CHART_SIZE.Z* AChart::CHORDS_SPACE_PERCENT_IN_CHART}};
const FString AChord::CHORD_MATERIAL_PATH{TEXT("/Game/StarterContent/Materials/M_Metal_Burnished_Steel")};
const FString AChord::CHORD_MESH_PATH{TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder")};
const float AChord::CHORD_POS_JUMP{AChart::CHART_SIZE.Z * 2 / AChart::MAX_CHORDS};

AChord::AChord() : index{0}, position{0.f}, hitbox{nullptr}, material{nullptr}, baseVisual{nullptr} {
    this->PrimaryActorTick.bCanEverTick = false;

    this->SetRootComponent(this->CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

    this->material = LoadObject<UMaterial>(nullptr, *AChord::CHORD_MATERIAL_PATH);
    this->baseVisual = LoadObject<UStaticMesh>(nullptr, *AChord::CHORD_MESH_PATH);

    if (!this->material) {
        this->material = UMaterial::GetDefaultMaterial(MD_Surface);
        UE_LOG(LogTemp, Warning, TEXT("AChord::AChord: Failed to load material"));
    }

    if (!this->baseVisual) {
        UE_LOG(LogTemp, Warning, TEXT("AChord::AChord: Failed to load mesh"));
        UE_LOG(LogTemp, Error, TEXT("AChord::AChord: Unable to create visual"));
    } else {
        this->SetActorScale3D(FVector{0.02f, 0.02f, AChart::CHART_SCALE.Y});
        this->visual = this->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("visual"));
        this->visual->SetStaticMesh(this->baseVisual);
        this->visual->SetMaterial(0, this->material);
        this->visual->SetRelativeLocation(FVector{0.f, 0.f, 0.f});
        this->visual->SetRelativeRotation(FRotator{0.f, 0.f, 90.0f});
        this->visual->SetupAttachment(this->GetRootComponent());
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

void AChord::setIndex(const uint8_t newIndex) {
    this->index = newIndex;
    this->position = AChord::CHORD_POS_JUMP * this->index;

    FVector newLocation{AChord::CHORD_BASE_POSITION - FVector{0.f, 0.f, this->position}};
    UE_LOG(LogTemp, Log, TEXT("AChord::setIndex: New location: %s"), *newLocation.ToString());
    this->SetActorLocation(newLocation);
}

bool AChord::handleHit() { return this->hitbox->verifyHit(); }

void AChord::createHitbox() {
    this->hitbox = GetWorld()->SpawnActor<AHitbox>(AHitbox::StaticClass());
    this->hitbox->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}
