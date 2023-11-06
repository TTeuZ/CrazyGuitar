#pragma once

// Standard includes
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
    ~AChartPawn();

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
    UPROPERTY(EditAnywhere)
    TArray<UStaticMeshComponent*> staticMeshes;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* hitBoxVisual;
};
