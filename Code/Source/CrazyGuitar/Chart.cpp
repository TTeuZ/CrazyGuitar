#include "Chart.h"

#include "Chord.h"

// Unreal includes
#include "Engine/EngineBaseTypes.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/Object.h"

const FVector AChart::CHART_SIZE{10.f, 300.f, 60.f};
const FVector AChart::CHART_SCALE{CHART_SIZE / 50.f};
const FVector AChart::CHART_INITIAL_LOCATION{200.f, 0.f, 250.f};
const FVector AChart::CAMERA_INITIAL_LOCATION{-AChart::CHART_SIZE.Y, 0.f, -AChart::CHART_SIZE.Z};
const FString AChart::CHART_NAME{TEXT("ChartComponent")};

AChart::AChart()
    : chords{nullptr, nullptr, nullptr, nullptr},
      notes{new Notes{}},
      boxVisualMaterial{nullptr},
      stringVisualMaterial{nullptr},
      hitBoxVisualMaterial{nullptr},
      boxVisual{nullptr},
      visibleComponent{nullptr},
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
    FVector cameraLocation{CAMERA_INITIAL_LOCATION};
    this->chartCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    this->chartCamera->SetRelativeLocation(cameraLocation);
    this->chartCamera->SetupAttachment(this->RootComponent);

    // Define player controller
    this->AutoPossessPlayer = EAutoReceiveInput::Player0;

    this->visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
    this->visibleComponent->SetupAttachment(this->RootComponent);
}

AChart::~AChart() { delete this->notes; }

void AChart::Tick(float deltaTime) { Super::Tick(deltaTime); }

void AChart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!PlayerInputComponent) return;

    PlayerInputComponent->BindAction("HitFirstChord", IE_Pressed, this, &AChart::hitFirstChord);
    PlayerInputComponent->BindAction("HitSecondChord", IE_Pressed, this, &AChart::hitSecondChord);
    PlayerInputComponent->BindAction("HitThirdChord", IE_Pressed, this, &AChart::hitThirdChord);
    PlayerInputComponent->BindAction("HitFourthChord", IE_Pressed, this, &AChart::hitFourthChord);

    PlayerInputComponent->BindAction("Start", IE_Released, this, &AChart::startGame);
}

void AChart::hitChord(const int8_t& chord) { this->notes->handleHit(chord); }

void AChart::BeginPlay() {
    Super::BeginPlay();
    this->SetActorLocation(CHART_INITIAL_LOCATION);

    this->createChords();
}

void AChart::createBoxVisual(UBoxComponent* const boxComponent, const FVector& rootLocation,
                                 const ConstructorHelpers::FObjectFinder<UStaticMesh>& boxVisualAsset) {
    FVector boxVisualScale{AChart::CHART_SCALE};

    boxComponent->SetRelativeLocation(rootLocation);
    boxComponent->SetBoxExtent(CHART_SIZE, true);

    // defining colision profile
    boxComponent->SetCollisionProfileName(TEXT("Pawn"));

    // Cria e posiciona um componente de malha (MeshComponent)
    this->boxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    boxVisual->SetupAttachment(boxComponent);
    if (boxVisualAsset.Succeeded()) {
        boxVisual->SetStaticMesh(boxVisualAsset.Object);
        boxVisual->SetRelativeLocation(FVector{0.0f, 0.0f, -CHART_SIZE.Z});
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
    FString chordName[4]{TEXT("Chord1"), TEXT("Chord2"), TEXT("Chord3"), TEXT("Chord4")};
    spawnParams.Owner = this;

    std::array<AChord*, 4>::iterator it{this->chords.begin()};
    for (uint8_t i{1}; it != this->chords.end(); ++it, ++i) {
        spawnParams.Name = *chordName[i - 1];

        (*it) = this->GetWorld()->SpawnActor<AChord>(AChord::StaticClass(), AChart::CHART_INITIAL_LOCATION,
                                                     FRotator{0.f, 0.f, 0.f}, FActorSpawnParameters{spawnParams});
        (*it)->setIndex(i - 1);
        UE_LOG(LogTemp, Log, TEXT("AChart::createStringVisual: Chord %d created"), i);
    }

    UE_LOG(LogTemp, Log, TEXT("AChart::createStringVisual: Chords created"));
}

void AChart::hitFirstChord() { this->hitChord(0); }
void AChart::hitSecondChord() { this->hitChord(1); }
void AChart::hitThirdChord() { this->hitChord(2); }
void AChart::hitFourthChord() { this->hitChord(3); }

void AChart::startGame() {
    this->notes->clearNoteActions();
    this->notes->createNotes(this->GetWorld());
    this->notes->startNotes();
}
