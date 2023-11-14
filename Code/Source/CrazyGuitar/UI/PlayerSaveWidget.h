#pragma once

// Unreal Includes
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

// Must be the last include
#include "PlayerSaveWidget.generated.h"

class UTextBlock;

UCLASS()
class CRAZYGUITAR_API UPlayerSaveWidget : public UUserWidget {
    GENERATED_BODY()

   public:
    UPlayerSaveWidget(const FObjectInitializer& ObjectInitializer);
    virtual ~UPlayerSaveWidget() = default;

    virtual void NativeConstruct() override;

    void updateDisplayedState(const float& score, const uint32_t& hits, const uint32_t& misses);

    void updateDisplayedStreak(const uint32_t& streak);

    // Need to be public so blueprint can read and write
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* scoreTxt;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* hitsTxt;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* missesTxt;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* streakTxt;
};
