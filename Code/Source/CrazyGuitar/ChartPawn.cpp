#include "ChartPawn.h"

// Unreal includes
#include "Components/BoxComponent.h"
#include "Materials/Material.h"
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

AChartPawn::~AChartPawn() { this->clearNoteActions(); }

void AChartPawn::BeginPlay() {
    Super::BeginPlay();

    this->SetActorLocation(CHART_INITIAL_LOCATION);
    float zJump = (CHART_SIZE.Z * 2) / (MAX_CHORDS + 1);
    FVector defaultLocation{CHART_INITIAL_LOCATION.X * 0.9f, 500.f, CHART_INITIAL_LOCATION.Z + CHART_SIZE.Z - zJump};

    // TODO: Move to notes class
    // replace new operator to use in unreal
    ANoteAction* noteAction1;
    noteAction1 = GetWorld()->SpawnActor<ANoteAction>(ANoteAction::StaticClass());
    noteAction1->setChord(0);
    noteAction1->setPosition(defaultLocation + FVector{0, 0, -zJump * noteAction1->getChord()});

    ANoteAction* noteAction2;
    noteAction2 = GetWorld()->SpawnActor<ANoteAction>(ANoteAction::StaticClass());
    noteAction2->setChord(1);
    noteAction2->setPosition(defaultLocation + FVector{0, 100.f, -zJump * noteAction2->getChord()});

    ANoteAction* noteAction3;
    noteAction3 = GetWorld()->SpawnActor<ANoteAction>(ANoteAction::StaticClass());
    noteAction3->setChord(2);
    noteAction3->setPosition(defaultLocation + FVector{0, 350.f, -zJump * noteAction3->getChord()});

    this->addNoteAction(noteAction1);
    this->addNoteAction(noteAction2);
    this->addNoteAction(noteAction3);
}

void AChartPawn::Tick(float deltaTime) { Super::Tick(deltaTime); }

void AChartPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!PlayerInputComponent) return;

    PlayerInputComponent->BindAction("HitFirstChord", IE_Pressed, this, &AChartPawn::hitFirstChord);
    PlayerInputComponent->BindAction("HitSecondChord", IE_Pressed, this, &AChartPawn::hitSecondChord);
    PlayerInputComponent->BindAction("HitThirdChord", IE_Pressed, this, &AChartPawn::hitThirdChord);
    PlayerInputComponent->BindAction("HitFourthChord", IE_Pressed, this, &AChartPawn::hitFourthChord);
}

void AChartPawn::hitChord(int8_t chord) {
    ANoteAction* noteAction{*this->noteActions.begin()};
    FVector location = noteAction->getPosition();

    UE_LOG(LogTemp, Warning, TEXT("HIT LOCATION %f"), location.Y);
    UE_LOG(LogTemp, Warning, TEXT("HIT CHORD %d"), chord + 1);

    // check if note is in hitbox
    bool ret = noteAction->isHit(chord, location.Y);

    if (ret) {
        // play note
        this->playNoteAction();
        // remove note from list
        this->popNoteAction();
        // destroy note
        noteAction->Destroy();

        UE_LOG(LogTemp, Log, TEXT("HIT CORRECT"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("HIT WRONG"));
    return; // Necessary?
}

void AChartPawn::addNoteAction(ANoteAction* noteAction) {
    noteActions.push_back(noteAction);
}

void AChartPawn::removeNoteAction(ANoteAction* noteAction) {
    noteActions.remove(noteAction);
}

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
    //  this->noteActions.clear();
}

void AChartPawn::playNoteAction() {
    // ANoteAction* noteAction(*this->noteActions.begin());
    // noteAction->play();
}

void AChartPawn::hitFirstChord() { this->hitChord(0); }
void AChartPawn::hitSecondChord() { this->hitChord(1); }
void AChartPawn::hitThirdChord() { this->hitChord(2); }
void AChartPawn::hitFourthChord() { this->hitChord(3); }
