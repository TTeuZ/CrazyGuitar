#include "Chart.h"

// Personal includes
#include "PlayerSave.h"

// Unreal includes
#include "Engine/EngineBaseTypes.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"

const FVector AChart::CHART_LOCATION{0.f, 0.f, 100.f};
const FVector AChart::CHART_SIZE{5.f, 650.f, 100.f};
const FVector AChart::CHART_SCALE{CHART_SIZE / 50.f};
const FString AChart::CHARD_MATERIAL_PATH{TEXT("/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut")};
const FString AChart::CHARD_MESH_PATH{TEXT("/Game/Shapes/Shape_Cube.Shape_Cube")};
const FRotator AChart::CHART_ROTATION{270.f, 0.f, 270.f};
const FVector AChart::CAMERA_LOCATION{-120.f, -AChart::CHART_SIZE.Y * 1.1f, 0.f};
const FRotator AChart::CAMERA_ROTATION{0.f, 75.f, 90.f};
const FString AChart::CHART_NAME{TEXT("ChartComponent")};

AChart::AChart()
    : chords{nullptr, nullptr, nullptr, nullptr},
      notes{new Notes{}},
      song{nullptr},
      material{nullptr},
      visual{nullptr},
      chartCamera{nullptr} {
    this->PrimaryActorTick.bCanEverTick = true;

    // Constructors helpers to build the chart
    static ConstructorHelpers::FObjectFinder<UStaticMesh> visualAsset{*AChart::CHARD_MESH_PATH};
    static ConstructorHelpers::FObjectFinder<UMaterial> materialLoader{*AChart::CHARD_MATERIAL_PATH};

    // Setting up the meterials
    if (materialLoader.Succeeded())
        this->material = materialLoader.Object;
    else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find wood material"));

    // Creating the default guitar component and setting as root (Unreal do not allow to use {} in this constructor)
    UBoxComponent* boxComponent = CreateDefaultSubobject<UBoxComponent>(*AChart::CHART_NAME);
    this->RootComponent = boxComponent;

    // Creating the visual items for the game
    this->createVisual(visualAsset);

    // Creating the camera
    this->chartCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    this->chartCamera->SetRelativeLocation(AChart::CAMERA_LOCATION);
    this->chartCamera->SetWorldRotation(AChart::CAMERA_ROTATION);
    this->chartCamera->SetupAttachment(this->RootComponent);
    this->RootComponent->SetRelativeRotation(AChart::CHART_ROTATION);
}

AChart::~AChart() { delete this->notes; }

void AChart::startGame() {
    this->song = new Song{"swallowYourSoul"};
    UE_LOG(LogTemp, Log, TEXT("AChart::startGame: Song loaded"));

    std::list<std::array<uint16_t, 3>> songNotes{this->song->getRawNotes()};
    if (songNotes.empty()) {
        UE_LOG(LogTemp, Error, TEXT("AChart::startGame: No notes found"));
        this->notes->createProceduralNotes();
    } else {
        UE_LOG(LogTemp, Log, TEXT("AChart::startGame: %d notes found"), songNotes.size());
        this->notes->createSongNotes(*this->song);
    }
    this->notes->startNotes();
}

void AChart::Tick(float deltaTime) { Super::Tick(deltaTime); }

void AChart::hitChord(const uint8_t& chord) {
    APlayerSave* playerState{static_cast<APlayerSave*>(this->GetPlayerState())};

    if (this->chords[chord]->handleHit())
        playerState->computeHit(APlayerSave::HIT_SCORE);
    else
        playerState->computeMiss();
}

void AChart::BeginPlay() {
    Super::BeginPlay();
    this->notes->setWorld(this->GetWorld());
    this->SetActorLocation(CHART_LOCATION);
    this->createChords();
}

void AChart::createVisual(const ConstructorHelpers::FObjectFinder<UStaticMesh>& visualAsset) {
    FVector visualScale{AChart::CHART_SCALE * FVector{1.f, 1.f, 1.f}};
    FVector rootLocation{0.f, 0.f, 0.f};
    UBoxComponent* boxComponent{static_cast<UBoxComponent*>(this->GetRootComponent())};

    boxComponent->SetRelativeLocation(rootLocation);
    boxComponent->SetBoxExtent(AChart::CHART_SIZE, true);

    // defining colision profile
    boxComponent->SetCollisionProfileName(TEXT("Pawn"));

    // Cria e posiciona um componente de malha (MeshComponent)
    this->visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    this->visual->SetupAttachment(boxComponent);
    if (visualAsset.Succeeded()) {
        this->visual->SetStaticMesh(visualAsset.Object);
        this->visual->SetRelativeLocation(FVector{0.f, 0.f, -AChart::CHART_SIZE.Z});
        // fit the box to the root component size
        this->visual->SetWorldScale3D(visualScale);
        this->visual->SetMaterial(0, this->material);
        this->visual->SetCastShadow(false);
    }
}

void AChart::createChords() {
    FActorSpawnParameters spawnParams;

    USceneComponent* chordRoot = this->RootComponent;
    std::array<AChord*, 4>::iterator it{this->chords.begin()};
    for (size_t i{1}; it != this->chords.end(); ++it, ++i) {
        (*it) = this->GetWorld()->SpawnActor<AChord>(AChord::StaticClass(), AChart::CHART_LOCATION,
                                                     FRotator{0.f, 0.f, 0.f}, spawnParams);
        (*it)->setIndex(i - 1);
        (*it)->AttachToComponent(chordRoot, FAttachmentTransformRules::KeepRelativeTransform);
        UE_LOG(LogTemp, Log, TEXT("AChart::createStringVisual: Chord %d created"), i);
    }

    UE_LOG(LogTemp, Log, TEXT("AChart::createStringVisual: Chords created"));
}
