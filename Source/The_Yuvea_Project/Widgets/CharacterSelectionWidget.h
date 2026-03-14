#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <The_Yuvea_Project/Characters/MainCharacter.h>
#include "MainMenuWidget.h"
#include "CharacterSelectionWidget.generated.h"

class ACharacterPreviewActor;
class UCharacterPreviewWidget;
class UBorder;

UCLASS()
class THE_YUVEA_PROJECT_API UCharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void SpawnPreviewActor();
	void UpdateCharacterPreview(USkeletalMesh* NewMesh);

	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	TSubclassOf<ACharacterPreviewActor> PreviewActorClass;

	UPROPERTY()
	UCharacterPreviewWidget* PreviewWidget;

	UPROPERTY()
	ACharacterPreviewActor* PreviewActor;

	UPROPERTY(meta = (BindWidget))
	UButton* EnterWorldButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
    
    UFUNCTION()
    void OnSelectCharacter1();

    UFUNCTION()
    void OnSelectCharacter2();

    UFUNCTION()
    void OnSelectCharacter3();
    
    UFUNCTION()
    void OnQuitGame();

    void HighlightSelectedCharacter(UBorder* SelectedBorder);

    UFUNCTION()
    void OnEnterWorld();

    void UpdatePreviewActor(TSubclassOf<ACharacter> CharacterClass);

    UPROPERTY(EditAnywhere, Category = "Characters")
    TSubclassOf<ACharacter> CharacterClass1;

    UPROPERTY(EditAnywhere, Category = "Characters")
    TSubclassOf<ACharacter> CharacterClass2;

    UPROPERTY(EditAnywhere, Category = "Characters")
    TSubclassOf<ACharacter> CharacterClass3;

    UPROPERTY(meta = (BindWidget))
    UButton* Character1Button;

    UPROPERTY(meta = (BindWidget))
    UButton* Character2Button;

    UPROPERTY(meta = (BindWidget))
    UButton* Character3Button;

    UPROPERTY(meta = (BindWidget))
    UBorder* Border_Character1;

    UPROPERTY(meta = (BindWidget))
    UBorder* Border_Character2;

    UPROPERTY(meta = (BindWidget))
    UBorder* Border_Character3;

    TSubclassOf<APawn> SelectedCharacterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Meshes")
    USkeletalMesh* Character1Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Meshes")
    USkeletalMesh* Character2Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Meshes")
    USkeletalMesh* Character3Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;
};
