#include "Hitbox.h"

// Unreal Includes
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

const FVector AHitbox::HITBOX_BASE_LOCATION{-500.f, 0.f, 0.f};
const FVector AHitbox::HITBOX_SIZE{475.f, 700.f, 2.5f};

AHitbox::AHitbox()
    : noteAction{nullptr},
      baseVisual{nullptr},
      material{nullptr},
      collisionBox{CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"))},
      visual{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitVisual"))} {
    this->PrimaryActorTick.bCanEverTick = true;

    this->SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

    this->collisionBox->SetBoxExtent(AHitbox::HITBOX_SIZE);
    this->collisionBox->SetRelativeLocation(AHitbox::HITBOX_BASE_LOCATION);
    this->collisionBox->SetCollisionProfileName(TEXT("Trigger"));
    this->collisionBox->SetupAttachment(this->RootComponent);

    this->collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHitbox::onOverlapBegin);
    this->collisionBox->OnComponentEndOverlap.AddDynamic(this, &AHitbox::onOverlapEnd);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> meshLoader{TEXT("/Game/Shapes/Shape_Cube.Shape_Cube")};
    static ConstructorHelpers::FObjectFinder<UMaterial> materialLoader{
        TEXT("/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor")};

    if (materialLoader.Succeeded())
        this->material = materialLoader.Object;
    else {
        this->material = UMaterial::GetDefaultMaterial(MD_Surface);
        UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox material"));
    }

    if (meshLoader.Succeeded()) {
        this->baseVisual = meshLoader.Object;

        this->visual->SetStaticMesh(this->baseVisual);
        this->visual->SetMaterial(0, this->material);
        this->visual->SetUsingAbsoluteScale(true);
        this->visual->SetWorldScale3D(FVector{0.05f, 0.15f, 0.25f});
        this->visual->SetRelativeLocation(FVector{-100.f, 0.f, -1.f});
        this->visual->SetupAttachment(this->RootComponent);
        this->visual->SetCollisionProfileName(TEXT("NoCollision"));
    } else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox mesh"));
}

void AHitbox::BeginPlay() { Super::BeginPlay(); }

void AHitbox::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AHitbox::onOverlapBegin(UPrimitiveComponent* const overlappedComp, AActor* const otherActor,
                             UPrimitiveComponent* const otherComp, const int32 otherBodyIndex, const bool bFromSweep,
                             const FHitResult& sweepResult) {
    this->noteAction = dynamic_cast<ANoteAction*>(otherActor);
}

void AHitbox::onOverlapEnd(UPrimitiveComponent* const overlappedComp, AActor* const otherActor,
                           UPrimitiveComponent* const otherComp, const int32 otherBodyIndex) {
    this->noteAction = nullptr;
}

bool AHitbox::verifyHit() {
    if (!this->noteAction) return false;
    this->noteAction->hit();
    return true;
}
