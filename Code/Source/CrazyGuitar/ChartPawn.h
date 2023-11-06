#pragma once

#include <cstdint>
#include <list>

#include "NoteAction.h"

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Pawn.h"

// Must be the last include
#include "ChartPawn.generated.h"

constexpr uint8_t MAX_CHORDS = 4;

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
    void createBoxVisual(void* boxComponentPtr, FVector boxLocation, FVector boxExtent, void* boxVisualAssetPtr);
    void createStringVisual(void* stringComponentPtr, FVector rootExtent, void* boxComponentPtr,
                            void* stringVisualAssetPtr);

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

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* boxVisual;
    UPROPERTY(EditAnywhere)
    USceneComponent* visibleComponent;
    UPROPERTY(EditAnywhere)
    UCameraComponent* chartCamera;
    UPROPERTY(EditAnywhere)
    TArray<UStaticMeshComponent*> staticMeshes;
};
