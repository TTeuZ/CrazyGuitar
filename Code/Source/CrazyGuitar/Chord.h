#pragma once

#include "CrazyGuitar/Hitbox.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Math/Vector.h"

// Must be the last include
#include "Chord.generated.h"

UCLASS()
class AChord : public AActor {
    GENERATED_BODY()

   public:
    const static FVector CHORD_BASE_POSITION;
    const static float CHORD_POS_JUMP;

    AChord();
    virtual ~AChord() = default;

    void setIndex(const uint8_t newIndex);

    bool handleHit();

   protected:
    void BeginPlay() override;

   private:
    const static FString CHORD_MATERIAL_PATH;
    const static FString CHORD_MESH_PATH;

    void createHitbox();

    uint8_t index;
    float position;
    AHitbox *hitbox;

    UPROPERTY(EditAnywhere)
    UMaterial *material;
    UPROPERTY(EditAnywhere)
    UStaticMesh *visual;
    UStaticMeshComponent *chordVisual;
};
