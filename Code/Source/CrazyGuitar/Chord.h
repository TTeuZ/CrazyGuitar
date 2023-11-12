#pragma once

#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Math/Vector.h"

// Must be the last include
#include "Chord.generated.h"

UCLASS()
class AChord : public AActor {
    GENERATED_BODY()

   public:
    static const FVector CHORD_BASE_POSITION;
    static const FVector CHORD_INITIAL_LOCATION;

    AChord();
    virtual ~AChord() = default;

    int getIndex() const;
    float getPosition() const;

    void setIndex(const uint8_t newIndex);

   protected:
    void BeginPlay() override;

   private:
    static constexpr float CHORDS_SPACE_PERCENT_IN_CHART{0.7f};
    static const FString CHORD_MATERIAL_PATH;
    static const FString CHORD_MESH_PATH;
    static const FVector CHORD_SCALE;
    static const FVector CHORD_SIZE;

    uint8_t index;
    void *notes;
    float position;

    UPROPERTY(EditAnywhere)
    UMaterial *material;
    UPROPERTY(EditAnywhere)
    UStaticMesh *visual;

    UStaticMeshComponent *chordVisual;
};
