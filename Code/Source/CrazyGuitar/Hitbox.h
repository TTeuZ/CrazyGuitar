// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hitbox.generated.h"

UCLASS()
class CRAZYGUITAR_API AHitbox : public AActor
{
	GENERATED_BODY()
	
public:	
	AHitbox();
    virtual ~AHitbox() = default;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	

};
