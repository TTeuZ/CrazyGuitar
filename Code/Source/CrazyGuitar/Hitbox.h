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

UCLASS() class CRAZYGUITAR_API AHitbox : public AActor {
    GENERATED_BODY()

   public:
    AHitbox();
    virtual ~AHitbox() = default;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void onOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp,
                        int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

    UFUNCTION()
    void onOverlapEnd(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp,
                      int32 otherBodyIndex);

    bool verifyHit();

   protected:
    virtual void BeginPlay() override;

   private:
    const static FVector HITBOX_BASE_LOCATION;
    const static FVector HITBOX_SIZE;

    ANoteAction* noteAction;

    UPROPERTY(VisibleAnywhere, Category = "Hitbox")
    class UBoxComponent* collisionBox;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* hitboxVisual;

    UStaticMesh* hitboxMesh;
    UMaterial* hitboxMaterial;
};
