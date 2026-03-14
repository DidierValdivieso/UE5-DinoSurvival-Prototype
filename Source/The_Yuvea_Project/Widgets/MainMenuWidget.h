#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSinglePlayerClicked);

class UCharacterSelectionWidget;
class USettingsWidget;
class UButton;

UCLASS()
class THE_YUVEA_PROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSinglePlayerClicked OnSinglePlayerClickedEvent;

protected:
	UFUNCTION()
	void OnSinglePlayerClicked();

	UFUNCTION()
	void OnMultiPlayerClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnQuitClicked();

	void NativeConstruct();

public:
	UPROPERTY(meta = (BindWidget))
	UButton* SinglePlayerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MultiPlayerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SettingsWidgetClass;

	USettingsWidget* SettingsWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UCharacterSelectionWidget> CharacterSelectionWidgetClass;

	UPROPERTY()
	UCharacterSelectionWidget* CharacterSelectionWidgetInstance;


};
