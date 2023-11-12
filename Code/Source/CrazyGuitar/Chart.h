#pragma once

// Standard includes
#include <array>
#include <cstdint>
#include <list>
#include <string>

// Personal includes
#include "Notes.h"
#include "Chord.h"

// Unreal includes
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

// Must be the last include
#include "Chart.generated.h"

UCLASS()
class CRAZYGUITAR_API AChart : public APawn {
    GENERATED_BODY()

   public:
    constexpr static uint8_t MAX_CHORDS{4};
    constexpr static float CHORDS_SPACE_PERCENT_IN_CHART{(AChart::MAX_CHORDS - 1.f)/AChart::MAX_CHORDS};
    const static FVector CHART_LOCATION;
    const static FRotator CHART_ROTATION;
    const static FVector CHART_SIZE;
    const static FVector CHART_SCALE;
    const static FString CHART_NAME;

    AChart();
    virtual ~AChart();

    std::array<AChord*, AChart::MAX_CHORDS> getChords() const;
    std::array<float, AChart::MAX_CHORDS> getChordsPositions() const;

    void startGame();

    virtual void Tick(float deltaTime) override;

    void hitChord(const int8_t& chord);

   protected:
    virtual void BeginPlay() override;

   private:
    const static FVector CAMERA_LOCATION;
    const static FRotator CAMERA_ROTATION;

    void createBoxVisual(UBoxComponent* const boxComponent, const FVector& rootLocation,
                         const ConstructorHelpers::FObjectFinder<UStaticMesh>& boxVisualAsset);
    void createHitboxVisual(UBoxComponent* const boxComponent,
                            const ConstructorHelpers::FObjectFinder<UStaticMesh>& cylinderVisualAsset);
    void createChords();

    std::array<AChord*, AChart::MAX_CHORDS> chords;
    Notes* notes;

    UMaterial* boxVisualMaterial;
    UMaterial* stringVisualMaterial;
    UMaterial* hitBoxVisualMaterial;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* boxVisual;
    UPROPERTY(EditAnywhere)
    UCameraComponent* chartCamera;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* hitBoxVisual;
};
