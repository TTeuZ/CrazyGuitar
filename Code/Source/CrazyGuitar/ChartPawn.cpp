#include "ChartPawn.h"

#include "Components/BoxComponent.h"
#include "Constants.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"

static constexpr uint8_t MAX_CHORDS(4);
static const FVector CHART_SIZE(10.f, 220.f, 60.f);
static const FVector CHART_SCALE(CHART_SIZE / 50.f);
static const FVector CHART_INITIAL_LOCATION(200.f, 0.f, 250.f);
static const FVector CAMERA_INITIAL_LOCATION(-220.f, 0.f, -50.f);

AChartPawn::AChartPawn() {
    PrimaryActorTick.bCanEverTick = true;

    this->noteSpeed = 1;
    this->noteActions = std::list<ANoteAction*>();
    this->boxVisualMaterial = nullptr;
    this->stringVisualMaterial = nullptr;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset(TEXT("/Game/Shapes/Shape_Cube.Shape_Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset(
        TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder"));

    static ConstructorHelpers::FObjectFinder<UMaterial> boxVisualMaterial(
        TEXT("/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut"));
    static ConstructorHelpers::FObjectFinder<UMaterial> stringVisualMaterial(
        TEXT("/Game/StarterContent/Materials/M_Metal_Burnished_Steel.M_Metal_Burnished_Steel"));
    static ConstructorHelpers::FObjectFinder<UMaterial> hitBoxVisualMaterial(TEXT("/Game/Materials/M_Hit_Box"));

    if (boxVisualMaterial.Succeeded()) {
        this->boxVisualMaterial = boxVisualMaterial.Object;
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find wood material"));
    }
    if (stringVisualMaterial.Succeeded()) {
        this->stringVisualMaterial = stringVisualMaterial.Object;
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find metal material"));
    }
    if (hitBoxVisualMaterial.Succeeded()) {
        this->hitBoxVisualMaterial = hitBoxVisualMaterial.Object;
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox material"));
    }

    // Define o controlador do jogador
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    // Cria o componente do braco da guitarra
    // Setando o componente como o componente raiz
    UBoxComponent* boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = boxComponent;

    // Define a localizacao e o tamanho do componente
    FVector rootLocation(0.f, 0.f, 0.f);

    this->createBoxVisual(boxComponent, rootLocation, &boxVisualAsset);
    this->createStringVisual(boxComponent, &cylinderVisualAsset);
    this->createHitboxVisual(boxComponent, &cylinderVisualAsset);

    this->visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
    this->chartCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    FVector cameraLocation(CAMERA_INITIAL_LOCATION);
    this->chartCamera->SetRelativeLocation(cameraLocation);
    this->chartCamera->SetupAttachment(RootComponent);

    this->visibleComponent->SetupAttachment(RootComponent);
}

AChartPawn::~AChartPawn() { this->clearNoteActions(); }

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
}

void AChartPawn::hitChord(int8_t chord) {
    // ANoteAction* noteAction(*this->noteActions.begin());
    // FVector location = noteAction->GetActorLocation();

    // check if note is in hitbox
    // bool ret = noteAction->isHit(0, location.X);
    bool ret = true;

    UE_LOG(LogTemp, Warning, TEXT("HIT CHORD %d"), chord + 1);
}

void AChartPawn::addNoteAction(ANoteAction* noteAction) { noteActions.push_back(noteAction); }

void AChartPawn::removeNoteAction(ANoteAction* noteAction) { noteActions.remove(noteAction); }

void AChartPawn::popNoteAction() { noteActions.pop_front(); }

void AChartPawn::createBoxVisual(void* boxComponentPtr, FVector rootLocation, void* boxVisualAssetPtr) {
    FVector boxVisualScale(CHART_SCALE);

    // Convertendo o ponteiro para o tipo correto
    ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)boxVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    boxComponent->SetRelativeLocation(rootLocation);
    boxComponent->SetBoxExtent(CHART_SIZE, true);

    // Define o perfil de colisao do componente
    boxComponent->SetCollisionProfileName(TEXT("Pawn"));

    // Cria e posiciona um componente de malha (MeshComponent)
    this->boxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    boxVisual->SetupAttachment(boxComponent);
    if (boxVisualAsset.Succeeded()) {
        boxVisual->SetStaticMesh(boxVisualAsset.Object);
        boxVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -CHART_SIZE.Z));
        // fit the box to the root component size
        boxVisual->SetWorldScale3D(boxVisualScale);
        boxVisual->SetMaterial(0, this->boxVisualMaterial);
        boxVisual->SetCastShadow(false);
    }
}

void AChartPawn::createStringVisual(void* boxComponentPtr, void* cylinderVisualAssetPtr) {
    // Convertendo o ponteiro para o tipo correto
    ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)cylinderVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    if (!cylinderVisualAsset.Succeeded()) {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create without mesh component"));
        return;
    }

    // Cria e posiciona quatro componentes de malha para representar as cordas
    // da guitarra. Cada componente é um cilindro
    for (int i = 0; i < MAX_CHORDS; ++i) {
        FVector stringBoxScale(0.02f, 0.02f, CHART_SCALE.Y);
        FVector stringLocation(-10.f, CHART_SIZE.Y, CHART_SIZE.Z);
        stringLocation.Z -= ((CHART_SIZE.Z * 2) / (MAX_CHORDS + 1)) * (i + 1);
        FRotator stringRotation(0.f, 0.f, 270.0f);
        FString stringName = FString::Printf(TEXT("String%d"), i);
        this->staticMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(*stringName));
        this->staticMeshes[i]->SetupAttachment(boxComponent);
        this->staticMeshes[i]->SetStaticMesh(cylinderVisualAsset.Object);
        this->staticMeshes[i]->SetRelativeLocationAndRotation(stringLocation, stringRotation);
        this->staticMeshes[i]->SetWorldScale3D(stringBoxScale);
        this->staticMeshes[i]->SetMaterial(0, this->stringVisualMaterial);
        this->staticMeshes[i]->SetCastShadow(false);
    }
}

void AChartPawn::createHitboxVisual(void* boxComponentPtr, void* cylinderVisualAssetPtr) {
    // Convertendo o ponteiro para o tipo correto
    ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)cylinderVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    if (cylinderVisualAsset.Succeeded()) {
        FVector hitBoxBoxScale(0.2f, 0.2f, CHART_SCALE.Z * 1.05f);
        FVector hitBoxLocation(-10.f, -154.f, -CHART_SIZE.Z * 1.05f);
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
