#include "SettingsWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.h"
#include <Kismet/GameplayStatics.h>
#include <The_Yuvea_Project/Controllers/MainPlayerController.h>

bool USettingsWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (MasterVolumeSlider)
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnMasterVolumeChanged);
    if (ResolutionDropdown)
        ResolutionDropdown->OnSelectionChanged.AddDynamic(this, &USettingsWidget::OnResolutionChanged);
    if (FullscreenCheckbox)
        FullscreenCheckbox->OnCheckStateChanged.AddDynamic(this, &USettingsWidget::OnFullscreenToggled);
    if (QualityDropdown)
        QualityDropdown->OnSelectionChanged.AddDynamic(this, &USettingsWidget::OnQualityChanged);
    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &USettingsWidget::OnQuitClicked);

    return true;
}

void USettingsWidget::OnMasterVolumeChanged(float Value)
{

}

void USettingsWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

void USettingsWidget::OnFullscreenToggled(bool bIsChecked)
{

}

void USettingsWidget::OnQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

void USettingsWidget::OnQuitClicked()
{
    RemoveFromParent();

    if (MainMenu)
    {
        MainMenu->SetVisibility(ESlateVisibility::Visible);

        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}

void USettingsWidget::SetMainMenu(UMainMenuWidget* InMainMenu)
{
    MainMenu = InMainMenu;
}