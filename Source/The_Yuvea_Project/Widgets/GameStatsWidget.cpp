#include "GameStatsWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../Characters/MainCharacter.h"
#include <The_Yuvea_Project/Actors/UltraDynamicSkyActorBase.h>
#include <Kismet/GameplayStatics.h>

void UGameStatsWidget::SetOwnerCharacter(AMainCharacter* Character)
{
    OwnerCharacter = Character;
}

void UGameStatsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!SkyActor)
    {
        SkyActor = Cast<AUltraDynamicSkyActorBase>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AUltraDynamicSkyActorBase::StaticClass()));
    }

    if (SkyActor && TimeOfDayText)
    {
        float Time = SkyActor->GetTimeOfDay();
        FString TimeString = FString::Printf(TEXT("Hour: %.2f h"), Time);
        TimeOfDayText->SetText(FText::FromString(TimeString));
    }

    if (OwnerCharacter)
    {
        float Current = OwnerCharacter->GetCurrentHydration();
        float Max = OwnerCharacter->GetMaxHydration();

        if (HydrationBar)
            HydrationBar->SetPercent(OwnerCharacter->GetHydrationRatio());

        if (HydrationText)
        {
            FString HydrationStr = FString::Printf(TEXT("Hydration: %.0f / %.0f"), Current, Max);
            HydrationText->SetText(FText::FromString(HydrationStr));
        }

        float CurrentEnergy = OwnerCharacter->GetCurrentEnergy();
        float MaxEnergy = OwnerCharacter->GetMaxEnergy();

        if (EnergyBar)
            EnergyBar->SetPercent(OwnerCharacter->GetEnergyRatio());

        if (EnergyText)
        {
            FString EnergyStr = FString::Printf(TEXT("Energy: %.0f / %.0f"), CurrentEnergy, MaxEnergy);
            EnergyText->SetText(FText::FromString(EnergyStr));
        }
    }

    if (OwnerCharacter)
    {
        float LifeRatio = OwnerCharacter->CurrentLife / OwnerCharacter->BaseMaxLife;

        if (LifeBar)
            LifeBar->SetPercent(LifeRatio);

        if (LifeText)
        {
            FString LifeStr = FString::Printf(TEXT("Life: %.0f / %.0f"), OwnerCharacter->CurrentLife, OwnerCharacter->BaseMaxLife);
            LifeText->SetText(FText::FromString(LifeStr));
        }
    }

    if (OwnerCharacter)
    {
        float CurrentBreath = OwnerCharacter->GetCurrentBreath();
        float MaxBreath = OwnerCharacter->GetMaxBreath();

        if (BreathBar)
            BreathBar->SetPercent(OwnerCharacter->GetBreathRatio());

        if (BreathText)
        {
            FString BreathStr = FString::Printf(TEXT("Breath: %.0f / %.0f"), CurrentBreath, MaxBreath);
            BreathText->SetText(FText::FromString(BreathStr));
        }
    }

    UpdateSpeedFromOwner();

}

void UGameStatsWidget::ShowWarningMessage(const FString& Message)
{
    if (WarningText)
    {
        WarningText->SetText(FText::FromString(Message));
        WarningText->SetVisibility(ESlateVisibility::Visible);
    }
}

void UGameStatsWidget::HideWarningMessage()
{
    if (WarningText)
    {
        WarningText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UGameStatsWidget::UpdateBreath(float CurrentBreath, float MaxBreath)
{
    if (BreathBar)
    {
        BreathBar->SetPercent(MaxBreath > 0.f ? CurrentBreath / MaxBreath : 0.f);
    }

    if (BreathText)
    {
        FString BreathString = FString::Printf(TEXT("Breath: %.0f / %.0f"), CurrentBreath, MaxBreath);
        BreathText->SetText(FText::FromString(BreathString));
    }
}

void UGameStatsWidget::UpdateBreathFromOwner()
{
    if (!OwnerCharacter) return;

    float Current = OwnerCharacter->GetCurrentBreath();
    float Max = OwnerCharacter->GetMaxBreath();
    UpdateBreath(Current, Max);
}

void UGameStatsWidget::UpdateSpeedFromOwner()
{
    if (!OwnerCharacter || !SpeedText) return;

    float Speed = OwnerCharacter->GetVelocity().Size();
    FString SpeedStr = FString::Printf(TEXT("Speed: %.1f cm/s"), Speed);
    SpeedText->SetText(FText::FromString(SpeedStr));
}
