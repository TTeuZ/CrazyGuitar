#pragma once

// Standard includes
#include <array>
#include <cstdint>
#include <list>
#include <string>

// Personal includes
#include "NoteAction.h"

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

    void addNoteAction(ANoteAction* noteAction); // TODO: Move to notes class (Necessary?)

    void removeNoteAction(ANoteAction* noteAction); // TODO: Move to notes class (Necessary?)

    void popNoteAction(); // TODO: Move to notes class (Necessary?)

    void hitChord(int8_t chord); // TODO: Move to notes class ?

   protected:
    virtual void BeginPlay() override;

   private:
    constexpr static uint8_t MAX_CHORDS{4};
    const FVector CHART_SIZE{10.f, 220.f, 60.f};
    const FVector CHART_SCALE{CHART_SIZE / 50.f};
    const FVector CHART_INITIAL_LOCATION{200.f, 0.f, 250.f};
    const FVector CAMERA_INITIAL_LOCATION{-220.f, 0.f, -50.f};

    void createBoxVisual(const void* const boxComponentPtr, const FVector& rootLocation, const void* const boxVisualAssetPtr);

    void createStringVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr);

    void createHitboxVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr);

    void clearNoteActions(); // TODO: Move to notes class

    void playNoteAction(); // TODO: Move to notes class

    void hitFirstChord();

    void hitSecondChord();

    void hitThirdChord();

    void hitFourthChord();

    // Member Data
    std::list<ANoteAction*> noteActions;
    int8_t noteSpeed;

    UMaterial* boxVisualMaterial;
    UMaterial* stringVisualMaterial;
    UMaterial* hitBoxVisualMaterial;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* boxVisual;
    UPROPERTY(EditAnywhere)
    USceneComponent* visibleComponent;
    UPROPERTY(EditAnywhere)
    UCameraComponent* chartCamera;
    std::array<UStaticMeshComponent*, 4> staticMeshes;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* hitBoxVisual;
};
