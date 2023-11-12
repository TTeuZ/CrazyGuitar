#include "Hitbox.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

const FVector AHitbox::HITBOX_BASE_LOCATION{-500.f, 0.f, 0.f};
const FVector AHitbox::HITBOX_SIZE{375.f, 700.f, 3.333f};
const FString AHitbox::HITBOX_NAME{"Hitbox"};

AHitbox::AHitbox()
    : collisionBox{CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"))},
      hitboxMesh{nullptr},
      hitboxMaterial{nullptr} {
    PrimaryActorTick.bCanEverTick = true;

    this->collisionBox->SetBoxExtent(AHitbox::HITBOX_SIZE);
    this->collisionBox->SetRelativeLocation(AHitbox::HITBOX_BASE_LOCATION);
    this->collisionBox->SetCollisionProfileName(TEXT("Trigger"));
    this->RootComponent = this->collisionBox;

    this->collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHitbox::onOverlapBegin);
    this->collisionBox->OnComponentEndOverlap.AddDynamic(this, &AHitbox::onOverlapEnd);
}

void AHitbox::BeginPlay() {
    Super::BeginPlay();

    //
}

void AHitbox::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    //
}

void AHitbox::onOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp,
                             int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Overlap Begin Function of Hitbox"));
}

void AHitbox::onOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp,
                           int32 otherBodyIndex) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, TEXT("Overlap End Function of Hitbox"));
}
