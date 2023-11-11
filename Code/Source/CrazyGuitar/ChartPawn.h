#pragma once

// Standard includes
#include <array>
#include <cstdint>
#include <list>
#include <string>

// Personal includes
#include "Notes.h"

// Unreal includes
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

// Must be the last include
#include "ChartPawn.generated.h"

UCLASS()
class CRAZYGUITAR_API AChartPawn : public APawn {
    GENERATED_BODY()

   public:
    constexpr static uint8_t MAX_CHORDS{4};
    const static FVector CHART_INITIAL_LOCATION;
    const static FVector CHART_SIZE;

    AChartPawn();
    virtual ~AChartPawn();

    virtual void Tick(float deltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void hitChord(const int8_t& chord);

   protected:
    virtual void BeginPlay() override;

   private:
    const static FVector CHART_SCALE;
    const static FVector CAMERA_INITIAL_LOCATION;

    void createBoxVisual(UBoxComponent* const boxComponent, const FVector& rootLocation,
                         const ConstructorHelpers::FObjectFinder<UStaticMesh>& boxVisualAsset);
    void createStringVisual(UBoxComponent* const boxComponent,
                            const ConstructorHelpers::FObjectFinder<UStaticMesh>& cylinderVisualAsset);
    void createHitboxVisual(UBoxComponent* const boxComponent,
                            const ConstructorHelpers::FObjectFinder<UStaticMesh>& cylinderVisualAsset);

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
