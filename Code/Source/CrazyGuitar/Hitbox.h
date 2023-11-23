#pragma once

// Personal Includes
#include "NoteAction.h"

// Unreal includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

// Must be the last include
#include "Hitbox.generated.h"

class UPrimitiveComponent;
class UBoxComponent;

UCLASS() class CRAZYGUITAR_API AHitbox : public AActor {
    GENERATED_BODY()

   public:
    AHitbox();
    virtual ~AHitbox() = default;

    virtual void Tick(float DeltaTime) override;

    // All this params are required, otherwise we got a compilation error
    UFUNCTION()
    void onOverlapBegin(UPrimitiveComponent* const overlappedComp, AActor* const otherActor,
                        UPrimitiveComponent* const otherComp, const int32 otherBodyIndex, const bool bFromSweep,
                        const FHitResult& sweepResult);
    UFUNCTION()
    void onOverlapEnd(UPrimitiveComponent* const overlappedComp, AActor* const otherActor,
                      UPrimitiveComponent* const otherComp, const int32 otherBodyIndex);

    bool verifyHit();

   protected:
    virtual void BeginPlay() override;

   private:
    const static FVector HITBOX_BASE_LOCATION;
    const static FVector HITBOX_SIZE;

    ANoteAction* noteAction;

    UStaticMesh* baseVisual;
    UMaterial* material;
    UPROPERTY(VisibleAnywhere, Category = "Hitbox")
    UBoxComponent* collisionBox;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* visual;
};
