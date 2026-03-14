#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <The_Yuvea_Project/Actors/UltraDynamicSkyActorBase.h>
#include <Components/TextBlock.h>
#include "GameStatsWidget.generated.h"

class UProgressBar;
class AMainCharacter;

UCLASS()
class THE_YUVEA_PROJECT_API UGameStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    void SetOwnerCharacter(AMainCharacter* Character);

    UPROPERTY(meta = (BindWidget))
    UProgressBar* EnergyBar;

    UPROPERTY()
    AUltraDynamicSkyActorBase* SkyActor;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimeOfDayText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HydrationText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* EnergyText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* LifeBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LifeText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WarningText;

    UFUNCTION(BlueprintCallable)
    void ShowWarningMessage(const FString& Message);

    void HideWarningMessage();

    UPROPERTY(meta = (BindWidget))
    UProgressBar* BreathBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* BreathText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SpeedText;

    void UpdateBreath(float CurrentBreath, float MaxBreath);
    void UpdateBreathFromOwner();
    void UpdateSpeedFromOwner();


protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HydrationBar;

private:
    UPROPERTY()
    AMainCharacter* OwnerCharacter;
};