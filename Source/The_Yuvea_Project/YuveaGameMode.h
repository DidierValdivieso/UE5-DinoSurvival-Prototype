#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/MainMenuWidget.h"
#include "Widgets/GameStatsWidget.h"
#include "YuveaGameMode.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API AYuveaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AYuveaGameMode();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameStatsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	void StartGameFromMenu();
	void SpawnSelectedCharacter();
	void StartGame();
	void SetupHUD();

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* OpenMenuSound;

protected:
	virtual void BeginPlay() override;	

private:
	UPROPERTY()
	UGameStatsWidget* GameStatsWidgetInstance;

	UPROPERTY()
	UMainMenuWidget* MainMenuWidgetInstance;
};
