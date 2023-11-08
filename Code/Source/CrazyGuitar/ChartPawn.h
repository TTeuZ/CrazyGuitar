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

    static constexpr uint8_t MAX_CHORDS{4};
    const FVector CHART_SIZE{10.f, 240.f, 60.f};
    const FVector CHART_SCALE{CHART_SIZE / 50.f};
    const FVector CHART_INITIAL_LOCATION{200.f, 0.f, 250.f};
    const FVector CAMERA_INITIAL_LOCATION{-250.f, 0.f, -50.f};

   public:
    AChartPawn();
    virtual ~AChartPawn() = default;

    virtual void Tick(float deltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void addNoteAction(ANoteAction* noteAction);

    void removeNoteAction(ANoteAction* noteAction);

    void popNoteAction();

    void hitChord(const int8_t chord);

   protected:
    virtual void BeginPlay() override;

   private:
    void createBoxVisual(void* boxComponentPtr, FVector rootLocation, void* boxVisualAssetPtr);
    void createStringVisual(void* boxComponentPtr, void* cylinderVisualAssetPtr);
    void createHitboxVisual(void* boxComponentPtr, void* cylinderVisualAssetPtr);

    void clearNoteActions();
    void playNoteAction();

    void hitFirstChord();
    void hitSecondChord();
    void hitThirdChord();
    void hitFourthChord();

    void setupTestGame();
    void startGame();

    std::list<ANoteAction*> noteActions;
    int8_t noteSpeed;

    UMaterial* boxVisualMaterial;
    UMaterial* stringVisualMaterial;
    UMaterial* hitBoxVisualMaterial;

    std::array<UStaticMeshComponent*, 4> staticMeshes;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* boxVisual;
    UPROPERTY(EditAnywhere)
    USceneComponent* visibleComponent;
    UPROPERTY(EditAnywhere)
    UCameraComponent* chartCamera;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* hitBoxVisual;
};
