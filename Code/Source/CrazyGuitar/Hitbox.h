#pragma once

// Personal Includes
#include "NoteAction.h"

// Unreal includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

// Must be the last include
#include "Hitbox.generated.h"

UCLASS()
class CRAZYGUITAR_API AHitbox : public AActor {
    GENERATED_BODY()

   public:
    const static FVector HITBOX_BASE_LOCATION;

    AHitbox();
    virtual ~AHitbox() = default;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void onOverlapBegin(class UPrimitiveComponent* overlappedComp, class AActor* otherActor,
                        class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep,
                        const FHitResult& sweepResult);

    UFUNCTION()
    void onOverlapEnd(class UPrimitiveComponent* overlappedComp, class AActor* otherActor,
                      class UPrimitiveComponent* otherComp, int32 otherBodyIndex);

    bool verifyHit();

   protected:
    virtual void BeginPlay() override;

   private:
    const static FVector HITBOX_SIZE;
    const static FString HITBOX_NAME;

    ANoteAction* noteAction;

    UPROPERTY(VisibleAnywhere, Category = "Hitbox")
    class UBoxComponent* collisionBox;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* hitboxVisual;

    UStaticMesh* hitboxMesh;
    UMaterial* hitboxMaterial;
};
