#include "SkillNodeWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/MainCharacter.h"

void USkillNodeWidget::NativeConstruct()
{
    Super::NativeConstruct();

    CharacterReference = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (!CharacterReference)
        return;

    FString SkillName;

    switch (SkillType)
    {
        case ESkillType::Constitution: {
            Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/head"));
            SkillName = TEXT("Constitution");
            if (Icon) {
                SkillIcon->SetBrushFromTexture(Icon);
            }
            if (SkillNameText)
            {
                SkillNameText->SetText(FText::FromString(SkillName));
            }
            if (CostText)
            {
                CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
            }
        }
        break;
    case ESkillType::IncreasedRegeneration: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/regeneration"));
        SkillName = TEXT("Increased\nRegeneration");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::RegenerationOfLifeSix: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/regeneration"));
        SkillName = TEXT("+6%\nRegeneration\nOf Life.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        
        break;
    case ESkillType::RegenerationOfLifeFifteen: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/regeneration"));
        SkillName = TEXT("+15%\nRegeneration\nOf Life.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        
        break;
    case ESkillType::Speedy: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/arrows"));
        SkillName = TEXT("Movement\nSpeed.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::MovementSpeedTen: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/arrows"));
        SkillName = TEXT("+10%\nMovement\nSpeed.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        
        break;
    case ESkillType::MovementSpeedTwenty: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/arrows"));
        SkillName = TEXT("+20%\nMovement\nSpeed.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        
        break;
    case ESkillType::AquaticBreath: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/snorkle"));
        SkillName = TEXT("Aquatic\nBreath");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::SecondsOfBreathingUnderwaterFive: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/snorkle"));
        SkillName = TEXT("+5\nSeconds\nOf Breathing\nUnderwater.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        
        break;
    case ESkillType::BreathingConsumptionRateFive: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/weather"));
        SkillName = TEXT("-5%\nBreathing\nConsumption\nRate.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::SecondsOfBreathingUnderwaterTen: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/snorkle"));
        SkillName = TEXT("+10\nSeconds\nOf Breathing\nUnderwater.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::BreathingConsumptionRateTen: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/weather"));
        SkillName = TEXT("-10%\nBreathing\nConsumption\nRate.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::StrongerStomach: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/stomach"));
        SkillName = TEXT("Stronger\nStomach");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::ResistanceToFoodPoisoningTen: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/stomach"));
        SkillName = TEXT("+10%\nResistance\nTo Food\nPoisoning.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::ResistanceToFoodPoisoningFifteen: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/stomach"));
        SkillName = TEXT("+15%\nResistance\nTo Food\nPoisoning.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::HigherHealthPool: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/healthcare"));
        SkillName = TEXT("Higher\nHealth\nPool");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::MaximumLifeTen: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/healthcare"));
        SkillName = TEXT("+10%\nMaximum\nLife.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    case ESkillType::MaximumLifeTwenty: {
        Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Materials/healthcare"));
        SkillName = TEXT("+20%\nMaximum\nLife.");
        if (Icon) {
            SkillIcon->SetBrushFromTexture(Icon);
        }
        if (SkillNameText)
        {
            SkillNameText->SetText(FText::FromString(SkillName));
        }
        if (CostText)
        {
            CostText->SetText(FText::FromString(FString::Printf(TEXT("Cost: %d"), Cost)));
        }
    }
        break;
    }

    NodeButton->OnPressed.AddDynamic(this, &USkillNodeWidget::OnButtonClicked);
}

void USkillNodeWidget::OnButtonClicked()
{
    if (!CharacterReference || bSkillUnlocked || CharacterReference->SkillPoints < Cost)
        return;

    bool bCanUnlock = PreviousSkills.Num() == 0;

    if (!bCanUnlock)
    {
        for (USkillNodeWidget* Element : PreviousSkills)
        {
            if (Element && Element->bSkillUnlocked)
            {
                bCanUnlock = true;
                break;
            }
        }
    }

    if (bCanUnlock)
    {
        UnlockSkill();
        UnlockAbility();
        bSkillUnlocked = true;
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Previous skill not unlocked."));
        }
    }
}

void USkillNodeWidget::UnlockAbility()
{
    switch (SkillType)
    {
    case ESkillType::Constitution: {}
        break;
    case ESkillType::IncreasedRegeneration: {}
        break;
    case ESkillType::RegenerationOfLifeSix:
        CharacterReference->BonusRegenPercent += 6.f;
        CharacterReference->ApplyRegenModifiers();
        break;
    case ESkillType::RegenerationOfLifeFifteen:
        CharacterReference->BonusRegenPercent += 15.f;
        CharacterReference->ApplyRegenModifiers();
        break;
    case ESkillType::Speedy: {}
        break;
    case ESkillType::MovementSpeedTen:
        CharacterReference->SpeedModifier = 0.10f;
        CharacterReference->ApplySpeedModifiers();
        break;
    case ESkillType::MovementSpeedTwenty:
        CharacterReference->SpeedModifier = 0.20f;
        CharacterReference->ApplySpeedModifiers();
        break;
    case ESkillType::AquaticBreath: {}
        break;
    case ESkillType::SecondsOfBreathingUnderwaterFive:
        CharacterReference->BonusBreathSeconds += 5.f;
        CharacterReference->ApplyBreathModifiers();
        break;
    case ESkillType::SecondsOfBreathingUnderwaterTen:
        CharacterReference->BonusBreathSeconds += 10.f;
        CharacterReference->ApplyBreathModifiers();
        break;
    case ESkillType::BreathingConsumptionRateFive:
        CharacterReference->BreathModifier -= 0.05f;
        CharacterReference->ApplyBreathModifierEffect();
        break;
    case ESkillType::BreathingConsumptionRateTen:
        CharacterReference->BreathModifier -= 0.10f;
        CharacterReference->ApplyBreathModifierEffect();
        break;
    case ESkillType::StrongerStomach: {}
        break;
    case ESkillType::ResistanceToFoodPoisoningTen: 
        CharacterReference->ResistanceFoodPoisoning = 10.0f;
        CharacterReference->ApplyResistanceFoodPoisoningEffect();
        break;
    case ESkillType::ResistanceToFoodPoisoningFifteen:
        CharacterReference->ResistanceFoodPoisoning = 15.0f;
        CharacterReference->ApplyResistanceFoodPoisoningEffect();
        break;
    case ESkillType::HigherHealthPool: {}
        break;
    case ESkillType::MaximumLifeTen:
        CharacterReference->BonusMaxLifePercent += 10.f;
        CharacterReference->ApplyLifeModifiers();
        break;
    case ESkillType::MaximumLifeTwenty:
        CharacterReference->BonusMaxLifePercent += 20.f;
        CharacterReference->ApplyLifeModifiers();
        break;
    default:
        break;
    }
}

void USkillNodeWidget::UnlockSkill()
{
    CharacterReference->SkillPoints -= Cost;

    FButtonStyle NewButtonStyle = NodeButton->GetStyle();

    NewButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
    NewButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor::White);

    NodeButton->SetStyle(NewButtonStyle);
}