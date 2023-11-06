#include "ChartPawn.h"

#include "Components/BoxComponent.h"
#include "Constants.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"

AChartPawn::AChartPawn() {
    PrimaryActorTick.bCanEverTick = true;

    this->noteSpeed = 1;
    this->noteActions = std::list<ANoteAction*>();
    this->boxVisualMaterial = nullptr;
    this->stringVisualMaterial = nullptr;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset(
        TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> stringVisualAsset(
        TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));

    static ConstructorHelpers::FObjectFinder<UMaterial> boxVisualMaterial(
        TEXT("/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut"));
    static ConstructorHelpers::FObjectFinder<UMaterial> stringVisualMaterial(
        TEXT("/Game/StarterContent/Materials/M_Metal_Chrome.M_Metal_Chrome"));

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

    // Define o controlador do jogador
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    // Cria o componente do braco da guitarra
    // Setando o componente como o componente raiz
    UBoxComponent* boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = boxComponent;

    // Define a localizacao e o tamanho do componente
    FVector rootLocation(0.f, 0.f, 0.f);
    FVector bodyBoxExtent(10.f, 200.f, 50.f);

    this->createBoxVisual(boxComponent, rootLocation, bodyBoxExtent, &boxVisualAsset);
    this->createStringVisual(boxComponent, bodyBoxExtent, boxComponent, &stringVisualAsset);



    this->visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
    this->chartCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    FVector cameraLocation(FVector(-250.f, 0.f, 0.f));
    this->chartCamera->SetRelativeLocation(cameraLocation);
    this->chartCamera->SetupAttachment(RootComponent);

    this->visibleComponent->SetupAttachment(RootComponent);
}

AChartPawn::~AChartPawn() { this->clearNoteActions(); }

void AChartPawn::BeginPlay() {
    Super::BeginPlay();
    SetActorLocation(FVector(200.f, 0.f, 120.f));
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
    bool ret = false;
    if (chord == 0) ret = true;

    if (ret) {
        // this->popNoteAction();
        // noteAction->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("CORRECT HIT"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("MISS HIT"));
}

void AChartPawn::addNoteAction(ANoteAction* noteAction) { noteActions.push_back(noteAction); }

void AChartPawn::removeNoteAction(ANoteAction* noteAction) { noteActions.remove(noteAction); }

void AChartPawn::popNoteAction() { noteActions.pop_front(); }

void AChartPawn::createBoxVisual(void* boxComponentPtr, FVector rootLocation, FVector bodyBoxExtent,
                                 void* boxVisualAssetPtr) {
    FVector boxVisualScale(0.1f, 4.f, 1.f);

    // Convertendo o ponteiro para o tipo correto
    ConstructorHelpers::FObjectFinder<UStaticMesh> boxVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)boxVisualAssetPtr);
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    boxComponent->SetRelativeLocation(rootLocation);
    boxComponent->SetBoxExtent(bodyBoxExtent, true);

    // Define o perfil de colisao do componente
    boxComponent->SetCollisionProfileName(TEXT("Pawn"));

    // Cria e posiciona um componente de malha (MeshComponent)
    this->boxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    boxVisual->SetupAttachment(boxComponent);
    if (boxVisualAsset.Succeeded()) {
        boxVisual->SetStaticMesh(boxVisualAsset.Object);
        boxVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -bodyBoxExtent.Z));
        // fit the box to the root component size
        boxVisual->SetWorldScale3D(boxVisualScale);
        boxVisual->SetMaterial(0, this->boxVisualMaterial);
    }
}

void AChartPawn::createStringVisual(void* stringComponentPtr, FVector rootExtent, void* boxComponentPtr,
                                    void* stringVisualAssetPtr) {
    // Convertendo o ponteiro para o tipo correto
    ConstructorHelpers::FObjectFinder<UStaticMesh> stringVisualAsset =
        *((ConstructorHelpers::FObjectFinder<UStaticMesh>*)stringVisualAssetPtr);
    UStaticMeshComponent* stringComponent = (UStaticMeshComponent*)stringComponentPtr;
    UBoxComponent* boxComponent = (UBoxComponent*)boxComponentPtr;

    // Cria e posiciona quatro componentes de malha para representar as cordas
    // da guitarra. Cada componente é um cilindro
    for (int i = 0; i < MAX_CHORDS; ++i) {
        if (stringVisualAsset.Succeeded()) {
            FVector stringBoxScale(0.02f, 0.02f, 4.f);
            FVector stringLocation(-5.f, rootExtent.Y,
                                   rootExtent.Z * 0.8 - 2 * i * (rootExtent.Z * 0.8) / (MAX_CHORDS - 1));
            FRotator stringRotation(0.f, 0.f, 270.0f);
            FString stringName = FString::Printf(TEXT("String%d"), i);
            this->staticMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(*stringName));
            this->staticMeshes[i]->SetupAttachment(boxComponent);
            this->staticMeshes[i]->SetStaticMesh(stringVisualAsset.Object);
            this->staticMeshes[i]->SetRelativeLocationAndRotation(stringLocation, stringRotation);
            this->staticMeshes[i]->SetWorldScale3D(stringBoxScale);
            this->staticMeshes[i]->SetMaterial(0, this->stringVisualMaterial);
        }
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
