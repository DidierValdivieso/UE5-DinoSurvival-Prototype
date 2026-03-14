#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.h"
#include "SkillNodeWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UENUM()
enum class ESkillType : uint8 {
    Constitution UMETA(DisplayName = "Constitution"),
    IncreasedRegeneration UMETA(DisplayName = "IncreasedRegeneration"),
    RegenerationOfLifeSix UMETA(DisplayName = "RegenerationOfLifeSix"),
    RegenerationOfLifeFifteen UMETA(DisplayName = "RegenerationOfLifeFifteen"),
    Speedy UMETA(DisplayName = "Speedy"),
    MovementSpeedTen UMETA(DisplayName = "MovementSpeedTen"),
    MovementSpeedTwenty UMETA(DisplayName = "MovementSpeedTwenty"),
    AquaticBreath UMETA(DisplayName = "AquaticBreath"),
    SecondsOfBreathingUnderwaterFive UMETA(DisplayName = "SecondsOfBreathingUnderwaterFive"),
    BreathingConsumptionRateFive UMETA(DisplayName = "BreathingConsumptionRateFive"),
    SecondsOfBreathingUnderwaterTen UMETA(DisplayName = "SecondsOfBreathingUnderwaterTen"),
    BreathingConsumptionRateTen UMETA(DisplayName = "BreathingConsumptionRateTen"),
    StrongerStomach UMETA(DisplayName = "StrongerStomach"),
    ResistanceToFoodPoisoningTen UMETA(DisplayName = "ResistanceToFoodPoisoningTen"),
    ResistanceToFoodPoisoningFifteen UMETA(DisplayName = "ResistanceToFoodPoisoningFifteen"),
    HigherHealthPool UMETA(DisplayName = "HigherHealthPool"),
    MaximumLifeTen UMETA(DisplayName = "MaximumLifeTen"),
    MaximumLifeTwenty UMETA(DisplayName = "MaximumLifeTwenty")
};

UCLASS()
class THE_YUVEA_PROJECT_API USkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
    virtual void NativeConstruct() override;
    
    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
    UButton* NodeButton;

    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
    UImage* SkillIcon;

    UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "UI")
    ESkillType SkillType;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SkillNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CostText;

    UPROPERTY(EditAnywhere, Category = "UI")
    int16 Cost;

    UPROPERTY(EditAnywhere, Category = "UI")
    TArray<USkillNodeWidget*> PreviousSkills;

    bool bDoOnce = true;

    bool bSkillUnlocked = false;

    class AMainCharacter* CharacterReference;

    UTexture2D* Icon;

    UFUNCTION()
    void OnButtonClicked();

    void UnlockAbility();
    void UnlockSkill();
};