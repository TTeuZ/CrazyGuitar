#pragma once

#include "Materials/Material.h"
#include "Math/Vector.h"
#include "GameFramework/Actor.h"

// Must be the last include
#include "Chord.generated.h"

UCLASS()
class AChord : public AActor {
    GENERATED_BODY()
       
   public:
    AChord();
    virtual ~AChord() = default;

    static const FVector CHORD_BASE_POSITION;

    void BeginPlay() override;

    int getIndex() const;
    void setIndex(const short int newIndex);

    UStaticMeshComponent *const getVisual() const;

   private:
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
