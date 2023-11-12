#include "Hitbox.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"


const FVector AHitbox::HITBOX_BASE_LOCATION{0.0f, 0.0f, 0.0f};
const FVector AHitbox::HITBOX_SIZE{10.0f, 10.0f, 10.0f};
const FString AHitbox::HITBOX_NAME{"Hitbox"};

AHitbox::AHitbox() : hitboxMesh{nullptr}, hitboxMaterial{nullptr} {
    PrimaryActorTick.bCanEverTick = true;

    this->collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    this->collisionBox->SetBoxExtent(HITBOX_SIZE);
    this->collisionBox->SetRelativeLocation(HITBOX_BASE_LOCATION);
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

void AHitbox::onOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Overlap Begin Function Called"));
}

void AHitbox::onOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Overlap End Function Called"));
}
