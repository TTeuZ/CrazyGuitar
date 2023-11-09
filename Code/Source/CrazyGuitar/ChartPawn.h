#pragma once

// Standard includes
#include <array>
#include <cstdint>
#include <list>
#include <string>

// Personal includes
#include "Constants.h"
#include "Notes.h"

// Unreal includes
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Materials/Material.h"

// Must be the last include
#include "ChartPawn.generated.h"

UCLASS()
class CRAZYGUITAR_API AChartPawn : public APawn {
    GENERATED_BODY()

   public:
    AChartPawn();
    virtual ~AChartPawn();

    virtual void Tick(float deltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void hitChord(const int8_t& chord);

   protected:
    virtual void BeginPlay() override;

   private:
    void createBoxVisual(const void* const boxComponentPtr, const FVector& rootLocation,
                         const void* const boxVisualAssetPtr);
    void createStringVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr);
    void createHitboxVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr);

    void hitFirstChord();
    void hitSecondChord();
    void hitThirdChord();
    void hitFourthChord();

    void startGame();

    Notes* notes;
    std::array<UStaticMeshComponent*, 4> staticMeshes;

    UMaterial* boxVisualMaterial;
    UMaterial* stringVisualMaterial;
    UMaterial* hitBoxVisualMaterial;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* boxVisual;
    UPROPERTY(EditAnywhere)
    USceneComponent* visibleComponent;
    UPROPERTY(EditAnywhere)
    UCameraComponent* chartCamera;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* hitBoxVisual;
};
