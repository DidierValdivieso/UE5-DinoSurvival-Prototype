#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "SettingsWidget.generated.h"

class UMainMenuWidget;

UCLASS()
class THE_YUVEA_PROJECT_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta = (BindWidget)) 
    class USlider* MasterVolumeSlider;

    UPROPERTY(meta = (BindWidget)) 
    class UComboBoxString* ResolutionDropdown;

    UPROPERTY(meta = (BindWidget)) 
    class UCheckBox* FullscreenCheckbox;

    UPROPERTY(meta = (BindWidget)) 
    class UComboBoxString* QualityDropdown;

    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnFullscreenToggled(bool bIsChecked);

    UFUNCTION()
    void OnQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

public:
    void OnQuitClicked();

    void SetMainMenu(UMainMenuWidget* InMainMenu);

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UMainMenuWidget* MainMenuWidgetInstance;

    UPROPERTY()
    UMainMenuWidget* MainMenu;
};
