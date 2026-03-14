#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterSelectionWidget.h"
#include "SettingsWidget.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (SinglePlayerButton)
		SinglePlayerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSinglePlayerClicked);
	if (MultiPlayerButton)
		MultiPlayerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnMultiPlayerClicked);
	if (SettingsButton)
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);
	if (QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);

	return true;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
}

void UMainMenuWidget::OnSinglePlayerClicked()
{
    if (!CharacterSelectionWidgetInstance && CharacterSelectionWidgetClass)
    {
        CharacterSelectionWidgetInstance = CreateWidget<UCharacterSelectionWidget>(GetWorld(), CharacterSelectionWidgetClass);
    }

    if (CharacterSelectionWidgetInstance)
    {
        CharacterSelectionWidgetInstance->AddToViewport();
        this->SetVisibility(ESlateVisibility::Hidden);

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(CharacterSelectionWidgetInstance->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}

void UMainMenuWidget::OnMultiPlayerClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Multiplayer not implemented."));
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void UMainMenuWidget::OnSettingsClicked()
{
    if (!SettingsWidgetInstance && SettingsWidgetClass)
    {
        SettingsWidgetInstance = CreateWidget<USettingsWidget>(GetWorld(), SettingsWidgetClass);
        SettingsWidgetInstance->SetMainMenu(this);
    }

    if (SettingsWidgetInstance)
    {
        SettingsWidgetInstance->AddToViewport();
        this->SetVisibility(ESlateVisibility::Hidden);

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(SettingsWidgetInstance->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}

