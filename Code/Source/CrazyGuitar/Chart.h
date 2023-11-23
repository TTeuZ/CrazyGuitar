#pragma once

// Standard includes
#include <array>
#include <cstdint>
#include <list>
#include <string>

// Personal includes
#include "Notes.h"
#include "Song.h"
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
    constexpr static float CHORDS_SPACE_PERCENT_IN_CHART{(AChart::MAX_CHORDS - 1.f) / AChart::MAX_CHORDS};
    const static FVector CHART_LOCATION;
    const static FVector CHART_SIZE;
    const static FVector CHART_SCALE;

    AChart();
    virtual ~AChart();

    void startGame();
    virtual void Tick(float deltaTime) override;
    void hitChord(const uint8_t& chord);

   protected:
    virtual void BeginPlay() override;

   private:
    const static FString CHARD_MATERIAL_PATH;
    const static FString CHARD_MESH_PATH;
    const static FRotator CHART_ROTATION;
    const static FVector CAMERA_LOCATION;
    const static FRotator CAMERA_ROTATION;
    const static FString CHART_NAME;

    void createVisual(const ConstructorHelpers::FObjectFinder<UStaticMesh>& visualAsset);
    void createChords();

    std::array<AChord*, AChart::MAX_CHORDS> chords;
    Notes* notes;
    Song* song;

    UMaterial* material;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* visual;
    UPROPERTY(EditAnywhere)
    UCameraComponent* chartCamera;
};
