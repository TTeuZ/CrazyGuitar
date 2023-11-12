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
#include "Chart.generated.h"

UCLASS()
class CRAZYGUITAR_API AChart : public APawn {
    GENERATED_BODY()

   public:
    AChart();
    virtual ~AChart() = default;

    virtual void Tick(float deltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void addNoteAction(ANoteAction* noteAction); // TODO: Move to notes class (Necessary?)
    void removeNoteAction(ANoteAction* noteAction); // TODO: Move to notes class (Necessary?)
    void popNoteAction(); // TODO: Move to notes class (Necessary?)
    void hitChord(int8_t chord); // TODO: Move to notes class ?
    

    static constexpr float CHART_CHORDS_SPACE{0.75f};
    static const FVector CHART_INITIAL_LOCATION;
    static const FVector CHART_SIZE;
    static const FVector CHART_SCALE;
    static const FString CHART_NAME;

   protected:
    virtual void BeginPlay() override;

   private:
    constexpr static uint8_t MAX_CHORDS{4};
    static const FVector CAMERA_INITIAL_LOCATION;

    void createBoxVisual(const void* const boxComponentPtr, const FVector& rootLocation, const void* const boxVisualAssetPtr);
    void createStringVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr);
    void createHitboxVisual(const void* const boxComponentPtr, const void* const cylinderVisualAssetPtr);

    void clearNoteActions(); // TODO: Move to notes class
    void playNoteAction(); // TODO: Move to notes class

    void hitFirstChord();
    void hitSecondChord();
    void hitThirdChord();
    void hitFourthChord();

    void setupTestGame();
    void startGame();

    int8_t noteSpeed; // TODO: Move to notes class
    std::list<ANoteAction*> noteActions; // TODO: Move to notes class

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
