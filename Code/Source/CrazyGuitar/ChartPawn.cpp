#include "ChartPawn.h"

// Unreal includes
#include "Components/BoxComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"

AChartPawn::AChartPawn() : boxVisualMaterial{nullptr}, stringVisualMaterial{nullptr}, hitBoxVisualMaterial{nullptr} {
    this->PrimaryActorTick.bCanEverTick = true;

    // Initial definition for chart member data
    this->noteSpeed = 1; // TODO: To be moved to Notes class
    this->noteActions = std::list<ANoteAction*>{};  // TODO: To be moved to Notes class
    this->staticMeshes = std::array<UStaticMeshComponent*, 4>{nullptr, nullptr, nullptr, nullptr};

    // Constructors helpers to build the chart
    static ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset{TEXT("/Game/Shapes/Shape_Cube.Shape_Cube")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset{TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder")};

    static ConstructorHelpers::FObjectFinder<UMaterial> boxVisualMaterialLoader{TEXT("/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut")};
    static ConstructorHelpers::FObjectFinder<UMaterial> stringVisualMaterialLoader{TEXT("/Game/StarterContent/Materials/" "M_Metal_Burnished_Steel.M_Metal_Burnished_Steel")};
    static ConstructorHelpers::FObjectFinder<UMaterial> hitBoxVisualMaterialLoader{TEXT("/Game/Materials/M_Hit_Box")};

    // Setting up the meterials
    if (boxVisualMaterialLoader.Succeeded()) this->boxVisualMaterial = boxVisualMaterialLoader.Object;
    else UE_LOG(LogTemp, Warning, TEXT("Cannot find wood material"));

    if (stringVisualMaterialLoader.Succeeded()) this->stringVisualMaterial = stringVisualMaterialLoader.Object;
    else UE_LOG(LogTemp, Warning, TEXT("Cannot find metal material"));
    
    if (hitBoxVisualMaterialLoader.Succeeded()) this->hitBoxVisualMaterial = hitBoxVisualMaterialLoader.Object;
    else UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox material"));

    // Defining component location and size
    FVector rootLocation{0.f, 0.f, 0.f};

    // Creating the default guitar component and setting as root (Unreal do not allow to use {} in this constructor)
    UBoxComponent* boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    this->RootComponent = boxComponent;

    // Creating the visual items for the game
    this->createBoxVisual(boxComponent, rootLocation, &boxVisualAsset);
    this->createStringVisual(boxComponent, &cylinderVisualAsset);
    this->createHitboxVisual(boxComponent, &cylinderVisualAsset);

    // Creating the camera
    FVector cameraLocation{CAMERA_INITIAL_LOCATION};
    this->chartCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); // (Unreal do not allow to use {} in this constructor)
    this->chartCamera->SetRelativeLocation(cameraLocation);
    this->chartCamera->SetupAttachment(this->RootComponent);

    // Define player controller
    this->AutoPossessPlayer = EAutoReceiveInput::Player0;

    // Creating the visible game component (Unreal do not allow to use {} in this constructor)
    this->visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
    this->visibleComponent->SetupAttachment(this->RootComponent);
}

void AChartPawn::BeginPlay() {
    Super::BeginPlay();
    SetActorLocation(CHART_INITIAL_LOCATION);
}

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

void AChartPawn::hitChord(int8_t chord) {
    std::list<ANoteAction*>::iterator it{this->noteActions.begin()};
    for (; it != this->noteActions.end(); ++it) {
        if (*it != nullptr) {
            FVector location = (*it)->getPosition();
            UE_LOG(LogTemp, Warning, TEXT("HIT LOCATION %f"), location.Y);
            UE_LOG(LogTemp, Warning, TEXT("HIT CHORD %d"), chord + 1);
            if ((*it)->isHit(chord, location.Y)) {
                // play note
                (*it)->playNote();

                // remove note from list
                ANoteAction* noteAction = *it;
                it = this->noteActions.erase(it);

                // destroy note
                noteAction->Destroy();

                UE_LOG(LogTemp, Log, TEXT("HIT CORRECT"));

                if (this->noteActions.empty()) {
                    UE_LOG(LogTemp, Log, TEXT("END OF GAME"));
                }
                return;
            } else
                UE_LOG(LogTemp, Log, TEXT("HIT WRONG"));
        }
    }
}

void AChartPawn::addNoteAction(ANoteAction* noteAction) { noteActions.push_back(noteAction); }

void AChartPawn::removeNoteAction(ANoteAction* noteAction) { noteActions.remove(noteAction); }

void AChartPawn::popNoteAction() { noteActions.pop_front(); }

void AChartPawn::createBoxVisual(const void* const boxComponentPtr, const FVector& rootLocation, const void* const boxVisualAssetPtr) {
    FVector boxVisualScale{CHART_SCALE};

    // Casting the pointers
    ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset = *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)boxVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    boxComponent->SetRelativeLocation(rootLocation);
    boxComponent->SetBoxExtent(CHART_SIZE, true);

    // defining colision profile
    boxComponent->SetCollisionProfileName(TEXT("Pawn"));

    // Cria e posiciona um componente de malha (MeshComponent) (Unreal do not allow to use {} in this constructor)
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

void AChartPawn::createStringVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr) {
    // Casting the pointers
    ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset = *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)cylinderVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    if (!cylinderVisualAsset.Succeeded()) {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create without mesh component"));
        return;
    }

    // Create 4 cylinders componenets to represent the chart strings
    std::array<UStaticMeshComponent*, 4>::iterator it{this->staticMeshes.begin()};
    for (size_t i{0}; it != this->staticMeshes.end(); ++i, ++it) {
        FVector stringBoxScale{0.02f, 0.02f, CHART_SCALE.Y};
        FVector stringLocation{-10.f, CHART_SIZE.Y, CHART_SIZE.Z};
        stringLocation.Z -= ((CHART_SIZE.Z * 2) / (MAX_CHORDS + 1)) * (i + 1);

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
    ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset = *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)cylinderVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

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

void AChartPawn::clearNoteActions() {
    for (ANoteAction* noteAction : this->noteActions) {
        noteAction->Destroy();
    }
    this->noteActions.clear();
}

void AChartPawn::hitFirstChord() { this->hitChord(0); }
void AChartPawn::hitSecondChord() { this->hitChord(1); }
void AChartPawn::hitThirdChord() { this->hitChord(2); }
void AChartPawn::hitFourthChord() { this->hitChord(3); }

void AChartPawn::setupTestGame() {
    float zJump = (CHART_SIZE.Z * 2) / (MAX_CHORDS + 1);
    FVector defaultLocation{CHART_INITIAL_LOCATION.X * 0.9f, 200.f, CHART_INITIAL_LOCATION.Z + CHART_SIZE.Z - zJump};

    // ANoteAction* noteAction1 = new ANoteAction(0, defaultLocation);
    // replace new operator to use in unreal
    ANoteAction* noteActionAux;
    ANoteAction* noteActionAux2;
    for (int32_t i{0}; i < 30; ++i) {
        noteActionAux = GetWorld()->SpawnActor<ANoteAction>();
        noteActionAux->setChord(FMath::RandRange(0, 3));
        noteActionAux2 = this->noteActions.back();
        float prevYLocation = 0.f;
        float min = 0.f;
        if (noteActionAux2 != nullptr) {
            prevYLocation = noteActionAux2->getPosition().Y;
            if (noteActionAux2->getChord() == noteActionAux->getChord()) {
                min = 30.f;
            }
        }
        float yLocation = prevYLocation + FMath::RandRange(min, 60.f);
        float zLocation = -zJump * noteActionAux->getChord();
        noteActionAux->setPosition(defaultLocation + FVector(0.f, yLocation, zLocation));
        this->addNoteAction(noteActionAux);
    }
}

void AChartPawn::startGame() {
    this->clearNoteActions();
    this->setupTestGame();
    std::list<ANoteAction*>::const_iterator it{this->noteActions.begin()};
    for (; it != this->noteActions.end(); ++it) {
        (*it)->setCanMove(true);
    }
}
