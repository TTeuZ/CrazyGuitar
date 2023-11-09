#include "ChartPawn.h"

// Unreal includes
#include "Components/BoxComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"

AChartPawn::AChartPawn()
    : notes{new Notes{}},
      staticMeshes{nullptr, nullptr, nullptr, nullptr},
      boxVisualMaterial{nullptr},
      stringVisualMaterial{nullptr},
      hitBoxVisualMaterial{nullptr} {
    this->PrimaryActorTick.bCanEverTick = true;

    // Constructors helpers to build the chart
    static ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset{TEXT("/Game/Shapes/Shape_Cube.Shape_Cube")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset{
        TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder")};

    static ConstructorHelpers::FObjectFinder<UMaterial> boxVisualMaterialLoader{
        TEXT("/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut")};
    static ConstructorHelpers::FObjectFinder<UMaterial> stringVisualMaterialLoader{
        TEXT("/Game/StarterContent/Materials/"
             "M_Metal_Burnished_Steel.M_Metal_Burnished_Steel")};
    static ConstructorHelpers::FObjectFinder<UMaterial> hitBoxVisualMaterialLoader{TEXT("/Game/Materials/M_Hit_Box")};

    // Setting up the meterials
    if (boxVisualMaterialLoader.Succeeded())
        this->boxVisualMaterial = boxVisualMaterialLoader.Object;
    else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find wood material"));

    if (stringVisualMaterialLoader.Succeeded())
        this->stringVisualMaterial = stringVisualMaterialLoader.Object;
    else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find metal material"));

    if (hitBoxVisualMaterialLoader.Succeeded())
        this->hitBoxVisualMaterial = hitBoxVisualMaterialLoader.Object;
    else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox material"));

    // Defining component location and size
    FVector rootLocation{0.f, 0.f, 0.f};

    UBoxComponent* boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    this->RootComponent = boxComponent;

    // Creating the visual items for the game
    this->createBoxVisual(boxComponent, rootLocation, &boxVisualAsset);
    this->createStringVisual(boxComponent, &cylinderVisualAsset);
    this->createHitboxVisual(boxComponent, &cylinderVisualAsset);

    // Creating the camera
    FVector cameraLocation{Constants::CAMERA_INITIAL_LOCATION};
    this->chartCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    this->chartCamera->SetRelativeLocation(cameraLocation);
    this->chartCamera->SetupAttachment(this->RootComponent);

    // Define player controller
    this->AutoPossessPlayer = EAutoReceiveInput::Player0;

    this->visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
    this->visibleComponent->SetupAttachment(this->RootComponent);
}

AChartPawn::~AChartPawn() { delete this->notes; }

void AChartPawn::Tick(float deltaTime) { Super::Tick(deltaTime); }

void AChartPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!PlayerInputComponent) return;

    PlayerInputComponent->BindAction("HitFirstChord", IE_Pressed, this, &AChartPawn::hitFirstChord);
    PlayerInputComponent->BindAction("HitSecondChord", IE_Pressed, this, &AChartPawn::hitSecondChord);
    PlayerInputComponent->BindAction("HitThirdChord", IE_Pressed, this, &AChartPawn::hitThirdChord);
    PlayerInputComponent->BindAction("HitFourthChord", IE_Pressed, this, &AChartPawn::hitFourthChord);

    PlayerInputComponent->BindAction("Start", IE_Released, this, &AChartPawn::startGame);
}

void AChartPawn::hitChord(const int8_t& chord) { this->notes->handleHit(chord); }

void AChartPawn::BeginPlay() {
    Super::BeginPlay();
    this->SetActorLocation(Constants::CHART_INITIAL_LOCATION);
}

void AChartPawn::createBoxVisual(const void* const boxComponentPtr, const FVector& rootLocation,
                                 const void* const boxVisualAssetPtr) {
    FVector boxVisualScale{Constants::CHART_SCALE};

    // Casting the pointers
    ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)boxVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    boxComponent->SetRelativeLocation(rootLocation);
    boxComponent->SetBoxExtent(Constants::CHART_SIZE, true);

    // defining colision profile
    boxComponent->SetCollisionProfileName(TEXT("Pawn"));

    // Cria e posiciona um componente de malha (MeshComponent) (Unreal do not
    // allow to use {} in this constructor)
    this->boxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    boxVisual->SetupAttachment(boxComponent);
    if (boxVisualAsset.Succeeded()) {
        boxVisual->SetStaticMesh(boxVisualAsset.Object);
        boxVisual->SetRelativeLocation(FVector{0.0f, 0.0f, -Constants::CHART_SIZE.Z});
        // fit the box to the root component size
        boxVisual->SetWorldScale3D(boxVisualScale);
        boxVisual->SetMaterial(0, this->boxVisualMaterial);
        boxVisual->SetCastShadow(false);
    }
}

void AChartPawn::createStringVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr) {
    // Casting the pointers
    ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)cylinderVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    if (!cylinderVisualAsset.Succeeded()) {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create without mesh component"));
        return;
    }

    // Create 4 cylinders componenets to represent the chart strings
    std::array<UStaticMeshComponent*, 4>::iterator it{this->staticMeshes.begin()};
    for (size_t i{0}; it != this->staticMeshes.end(); ++i, ++it) {
        FVector stringBoxScale{0.02f, 0.02f, Constants::CHART_SCALE.Y};
        FVector stringLocation{-10.f, Constants::CHART_SIZE.Y, Constants::CHART_SIZE.Z};
        stringLocation.Z -= ((Constants::CHART_SIZE.Z * 2) / (Constants::MAX_CHORDS + 1)) * (i + 1);

        FRotator stringRotation{0.f, 0.f, 270.0f};
        FString stringName = FString::Printf(TEXT("String%d"), i);

        (*it) = CreateDefaultSubobject<UStaticMeshComponent>(*stringName);
        (*it)->SetupAttachment(boxComponent);
        (*it)->SetStaticMesh(cylinderVisualAsset.Object);
        (*it)->SetRelativeLocationAndRotation(stringLocation, stringRotation);
        (*it)->SetWorldScale3D(stringBoxScale);
        (*it)->SetMaterial(0, this->stringVisualMaterial);
        (*it)->SetCastShadow(false);
    }
}

void AChartPawn::createHitboxVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr) {
    // Casting the pointers
    ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)cylinderVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    if (cylinderVisualAsset.Succeeded()) {
        FVector hitBoxBoxScale{0.2f, 0.2f, Constants::CHART_SCALE.Z * 1.05f};
        FVector hitBoxLocation{-10.f, -154.f, -Constants::CHART_SIZE.Z * 1.05f};
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

void AChartPawn::hitFirstChord() { this->hitChord(0); }
void AChartPawn::hitSecondChord() { this->hitChord(1); }
void AChartPawn::hitThirdChord() { this->hitChord(2); }
void AChartPawn::hitFourthChord() { this->hitChord(3); }

void AChartPawn::startGame() {
    this->notes->clearNoteActions();
    this->notes->createNotes(this->GetWorld());
    this->notes->startNotes();
}
