// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

// Must be the last include
#include "Hitbox.generated.h"

UCLASS()
class CRAZYGUITAR_API AHitbox : public AActor
{
	GENERATED_BODY()
	
public:	
	AHitbox();
    virtual ~AHitbox() = default;

	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
        void onOverlapBegin(class UPrimitiveComponent* overlappedComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

    UFUNCTION()
        void onOverlapEnd(class UPrimitiveComponent* overlappedComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex);

protected:
	virtual void BeginPlay() override;

private:	
    const static FVector HITBOX_BASE_LOCATION;
    const static FVector HITBOX_SIZE;
    const static FString HITBOX_NAME;

    UPROPERTY(VisibleAnywhere, Category = "Hitbox")
        class UBoxComponent* collisionBox;

    UStaticMesh* hitboxMesh;
    UMaterial* hitboxMaterial;
};
