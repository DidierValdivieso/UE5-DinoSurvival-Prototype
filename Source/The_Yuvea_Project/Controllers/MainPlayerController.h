#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <The_Yuvea_Project/Widgets/MainMenuWidget.h>
#include <The_Yuvea_Project/Widgets/CharacterPreviewWidget.h>
#include "MainPlayerController.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AMainPlayerController();

    UPROPERTY(EditAnywhere, Category = "SkillPoints")
    int16 SkillPoints;

    void ShowMainMenu();

    void HideMainMenu();

    UFUNCTION()
    void OnSinglePlayerStart();

    void SetupGameplayInput();

    UPROPERTY()
    USoundBase* MenuMusic;

    UPROPERTY()
    USoundBase* GameplayMusic;

    UPROPERTY()
    UAudioComponent* CurrentMusicComponent;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> SettingsWidgetClass;

    UUserWidget* SettingsWidgetInstance;

    bool bIsSettingsOpen = false;

protected:
    virtual void SetupInputComponent() override;
    void ToggleCharacterSelection();
    void TogglePreviewCharacter();
    void ToggleSettingsMenu();
    virtual void BeginPlay() override;

    UPROPERTY()
    class USkillTreeWidget* SkillTreeWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> SkillTreeWidgetClass;

    bool bIsSkillTreeVisible = false;

    void ToggleSkillTree();    

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> CharacterPreviewWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> CharacterSelectionWidgetClass;

    UMainMenuWidget* MainMenuWidget;
};
