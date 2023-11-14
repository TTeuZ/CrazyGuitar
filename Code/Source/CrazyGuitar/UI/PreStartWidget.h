#pragma once

// Unreal Includes
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

// Must be the last include
#include "PreStartWidget.generated.h"

UCLASS()
class CRAZYGUITAR_API UPreStartWidget : public UUserWidget {
    GENERATED_BODY()

   public:
    UPreStartWidget(const FObjectInitializer& ObjectInitializer);
    virtual ~UPreStartWidget() = default;

    virtual void NativeConstruct() override;

    // Need to be public so blueprint can read and write
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* titleTxt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* infoTxt;

   private:
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* fadeInOut;
};
