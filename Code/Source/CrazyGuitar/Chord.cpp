#include "Chord.h"

#include "Chart.h"

const FVector AChord::CHORD_BASE_POSITION{FVector{-10.0f, AChart::CHART_SIZE.Y, AChart::CHART_SIZE.Z * 0.9f}};
const FString AChord::CHORD_MATERIAL_PATH{TEXT("/Game/StarterContent/Materials/M_Metal_Burnished_Steel")};
const FString AChord::CHORD_MESH_PATH{TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder")};
const FVector AChord::CHORD_SCALE{FVector{0.02f, 0.02f, AChart::CHART_SCALE.Y}};
const FVector AChord::CHORD_SIZE{AChord::CHORD_SCALE * 50.f};

static const float CHORD_POS_JUMP{AChord::CHORD_BASE_POSITION.Z / 4 / 2};

AChord::AChord() : index{0}, notes{nullptr}, position{0.f}, material{nullptr}, visual{nullptr} {
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
    }
    else {
        this->SetActorScale3D(FVector{0.02f, 0.02f, AChart::CHART_SCALE.Y});
        this->chordVisual = this->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChordVisual"));
        this->chordVisual->SetStaticMesh(this->visual);
        this->chordVisual->SetMaterial(0, this->material);
        this->chordVisual->SetRelativeLocation(FVector{-AChart::CHART_SIZE.X, 0.f, 0.f});
        this->chordVisual->SetupAttachment(this->GetRootComponent());
        this->chordVisual->SetCastShadow(false);
    }

    this->SetActorRelativeRotation(FRotator{0.f, 0.f, 90.f});

    UE_LOG(LogTemp, Log, TEXT("AChord::AChord: Chord created"));
}

void AChord::BeginPlay() {
    Super::BeginPlay();

    this->SetActorLocation(AChord::CHORD_BASE_POSITION + FVector{0.f, 0.f, this->position});
}


int AChord::getIndex() const { return this->index; }

void AChord::setIndex(const short int newIndex) { 
    this->index = newIndex; 
    this->position = CHORD_POS_JUMP * this->index;
    this->SetActorLocation(AChord::CHORD_BASE_POSITION + FVector{0.f, 0.f, this->position});
}

UStaticMeshComponent *const AChord::getVisual() const {
    return this->chordVisual;
}
