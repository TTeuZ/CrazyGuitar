#include "Chart.h"

#include "Chord.h"

// Unreal includes
#include "Engine/EngineBaseTypes.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/Object.h"

const FVector AChart::CHART_SIZE{5.f, 300.f, 60.f};
const FVector AChart::CHART_SCALE{CHART_SIZE / 50.f};
const FVector AChart::CHART_LOCATION{0.f, 0.f, 100.f};
const FRotator AChart::CHART_ROTATION{270.f, 0.f, 270.f};
const FVector AChart::CAMERA_LOCATION{-60.f, -AChart::CHART_SIZE.Y, 0.f};
const FRotator AChart::CAMERA_ROTATION{0.f, 85.f, 90.f};
const FString AChart::CHART_NAME{TEXT("ChartComponent")};

AChart::AChart()
    : chords{nullptr, nullptr, nullptr, nullptr},
      notes{new Notes{}},
      boxVisualMaterial{nullptr},
      stringVisualMaterial{nullptr},
      hitBoxVisualMaterial{nullptr},
      boxVisual{nullptr},
      chartCamera{nullptr},
      hitBoxVisual{nullptr} {
    this->PrimaryActorTick.bCanEverTick = true;

    // Constructors helpers to build the chart
    static ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset{TEXT("/Game/Shapes/Shape_Cube.Shape_Cube")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset{
        TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder")};

    static ConstructorHelpers::FObjectFinder<UMaterial> boxVisualMaterialLoader{
        TEXT("/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut")};
    static ConstructorHelpers::FObjectFinder<UMaterial> hitBoxVisualMaterialLoader{TEXT("/Game/Materials/M_Hit_Box")};

    // Setting up the meterials
    if (boxVisualMaterialLoader.Succeeded())
        this->boxVisualMaterial = boxVisualMaterialLoader.Object;
    else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find wood material"));

    if (hitBoxVisualMaterialLoader.Succeeded())
        this->hitBoxVisualMaterial = hitBoxVisualMaterialLoader.Object;
    else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox material"));

    // Defining component location and size
    FVector rootLocation{0.f, 0.f, 0.f};

    // Creating the default guitar component and setting as root (Unreal do not allow to use {} in this constructor)
    UBoxComponent* boxComponent = CreateDefaultSubobject<UBoxComponent>(*CHART_NAME);
    this->RootComponent = boxComponent;

    // Creating the visual items for the game
    this->createBoxVisual(boxComponent, rootLocation, boxVisualAsset);
    this->createHitboxVisual(boxComponent, cylinderVisualAsset);

    // Creating the camera
    this->chartCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    this->chartCamera->SetRelativeLocation(AChart::CAMERA_LOCATION);
    this->chartCamera->SetWorldRotation(AChart::CAMERA_ROTATION);
    this->chartCamera->SetupAttachment(this->RootComponent);

    this->RootComponent->SetRelativeRotation(AChart::CHART_ROTATION);

    // Define player controller
    this->AutoPossessPlayer = EAutoReceiveInput::Player0;
}

AChart::~AChart() { delete this->notes; }

void AChart::startGame() {
    this->notes->clearNoteActions();
    this->notes->createNotes(this->GetWorld());
    this->notes->startNotes();
}

void AChart::Tick(float deltaTime) { 
    Super::Tick(deltaTime); 
}

void AChart::hitChord(const int8_t& chord) { this->notes->handleHit(chord); }

void AChart::BeginPlay() {
    Super::BeginPlay();
    this->SetActorLocation(CHART_LOCATION);

    this->createChords();
}

void AChart::createBoxVisual(UBoxComponent* const boxComponent, const FVector& rootLocation,
                             const ConstructorHelpers::FObjectFinder<UStaticMesh>& boxVisualAsset) {
    float visualMultiplier{3.f};
    FVector boxVisualScale{AChart::CHART_SCALE * FVector{1.f, visualMultiplier, 1.f}};

    boxComponent->SetRelativeLocation(rootLocation);
    boxComponent->SetBoxExtent(AChart::CHART_SIZE, true);

    // defining colision profile
    boxComponent->SetCollisionProfileName(TEXT("Pawn"));

    // Cria e posiciona um componente de malha (MeshComponent)
    this->boxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    boxVisual->SetupAttachment(boxComponent);
    if (boxVisualAsset.Succeeded()) {
        boxVisual->SetStaticMesh(boxVisualAsset.Object);
        boxVisual->SetRelativeLocation(FVector{0.0f, AChart::CHART_SIZE.Y * (visualMultiplier-1), -AChart::CHART_SIZE.Z});
        // fit the box to the root component size
        boxVisual->SetWorldScale3D(boxVisualScale);
        boxVisual->SetMaterial(0, this->boxVisualMaterial);
        boxVisual->SetCastShadow(false);
    }
}

void AChart::createHitboxVisual(UBoxComponent* const boxComponent,
                                const ConstructorHelpers::FObjectFinder<UStaticMesh>& cylinderVisualAsset) {
    if (cylinderVisualAsset.Succeeded()) {
        FVector hitBoxBoxScale{0.2f, 0.2f, CHART_SCALE.Z * 1.05f};
        FVector hitBoxLocation{-10.f, -154.f, -CHART_SIZE.Z * 1.05f};
        FString hitBoxName = FString::Printf(TEXT("HitBox"));

        this->hitBoxVisual = CreateDefaultSubobject<UStaticMeshComponent>(*hitBoxName);
        this->hitBoxVisual->SetupAttachment(boxComponent);
        this->hitBoxVisual->SetStaticMesh(cylinderVisualAsset.Object);
        this->hitBoxVisual->SetRelativeLocation(hitBoxLocation);
        this->hitBoxVisual->SetWorldScale3D(hitBoxBoxScale);
        this->hitBoxVisual->SetMaterial(0, this->hitBoxVisualMaterial);
        this->hitBoxVisual->SetCastShadow(false);
    }
}

void AChart::createChords() {
    AChord* chord;
    FActorSpawnParameters spawnParams;

    USceneComponent *chordRoot = this->RootComponent;

    std::array<AChord*, 4>::iterator it{this->chords.begin()};
    for (uint8_t i{1}; it != this->chords.end(); ++it, ++i) {
        (*it) = this->GetWorld()->SpawnActor<AChord>(AChord::StaticClass(), AChart::CHART_LOCATION,
                                                     FRotator{0.f, 0.f, 0.f}, spawnParams);
        (*it)->setIndex(i - 1);
        (*it)->AttachToComponent(chordRoot, FAttachmentTransformRules::KeepRelativeTransform);
        UE_LOG(LogTemp, Log, TEXT("AChart::createStringVisual: Chord %d created"), i);
    }

    UE_LOG(LogTemp, Log, TEXT("AChart::createStringVisual: Chords created"));
}
