#include "Hitbox.h"

// Unreal Includes
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

const FVector AHitbox::HITBOX_BASE_LOCATION{-500.f, 0.f, 0.f};
const FVector AHitbox::HITBOX_SIZE{375.f, 700.f, 2.5f};

AHitbox::AHitbox()
    : noteAction{nullptr},
      collisionBox{CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"))},
      hitboxVisual{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitboxVisual"))},
      hitboxMesh{nullptr},
      hitboxMaterial{nullptr} {
    PrimaryActorTick.bCanEverTick = true;

    this->SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

    this->collisionBox->SetBoxExtent(AHitbox::HITBOX_SIZE);
    this->collisionBox->SetRelativeLocation(AHitbox::HITBOX_BASE_LOCATION);
    this->collisionBox->SetCollisionProfileName(TEXT("Trigger"));
    this->collisionBox->SetupAttachment(this->RootComponent);

    this->collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHitbox::onOverlapBegin);
    this->collisionBox->OnComponentEndOverlap.AddDynamic(this, &AHitbox::onOverlapEnd);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> hitboxMeshLoader{TEXT("/Game/Shapes/Shape_Cube.Shape_Cube")};
    static ConstructorHelpers::FObjectFinder<UMaterial> hitboxMaterialLoader{
        TEXT("/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor")};

    if (hitboxMaterialLoader.Succeeded())
        this->hitboxMaterial = hitboxMaterialLoader.Object;
    else {
        this->hitboxMaterial = UMaterial::GetDefaultMaterial(MD_Surface);
        UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox material"));
    }

    if (hitboxMeshLoader.Succeeded()) {
        this->hitboxMesh = hitboxMeshLoader.Object;

        this->hitboxVisual->SetStaticMesh(this->hitboxMesh);
        this->hitboxVisual->SetMaterial(0, this->hitboxMaterial);
        this->hitboxVisual->SetUsingAbsoluteScale(true);
        this->hitboxVisual->SetWorldScale3D(FVector{0.05f, 0.15f, 0.25f});
        this->hitboxVisual->SetRelativeLocation(FVector{-100.f, 0.f, -0.25f * 4.85f});
        this->hitboxVisual->SetupAttachment(this->RootComponent);
        this->hitboxVisual->SetCollisionProfileName(TEXT("NoCollision"));
    } else
        UE_LOG(LogTemp, Warning, TEXT("Cannot find hitbox mesh"));
}

void AHitbox::BeginPlay() { Super::BeginPlay(); }

void AHitbox::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AHitbox::onOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp,
                             int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    ANoteAction* note{dynamic_cast<ANoteAction*>(otherActor)};
    this->noteAction = note;
}

void AHitbox::onOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp,
                           int32 otherBodyIndex) {
    // GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("Overlap End Function of Hitbox"));
    this->noteAction = nullptr;
}

bool AHitbox::verifyHit() {
    if (this->noteAction) {
        this->noteAction->hit();
        return true;
    } else
        return false;
}
