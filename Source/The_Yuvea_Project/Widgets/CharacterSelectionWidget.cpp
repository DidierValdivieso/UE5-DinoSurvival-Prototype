#include "CharacterSelectionWidget.h"
#include "CharacterPreviewWidget.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "../Actors/CharacterPreviewActor.h"
#include "../YuveaGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include <Kismet/GameplayStatics.h>
#include <The_Yuvea_Project/Controllers/MainPlayerController.h>
#include <The_Yuvea_Project/YuveaGameMode.h>
#include "Components/Border.h"

void UCharacterSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

    Character1Button->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnSelectCharacter1);
    Character2Button->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnSelectCharacter2);
    Character3Button->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnSelectCharacter3);

    EnterWorldButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnEnterWorld);
    QuitButton->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnQuitGame);

    SpawnPreviewActor();
    OnSelectCharacter1();	

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}

void UCharacterSelectionWidget::SpawnPreviewActor()
{
    if (PreviewActor && !PreviewActor->IsPendingKillPending())
    {
        return;
    }

    if (!PreviewActorClass || !GetWorld()) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FVector SpawnLocation = FVector(25329.0f, -14351.0f, -11384.0f);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    PreviewActor = GetWorld()->SpawnActor<ACharacterPreviewActor>(PreviewActorClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (PreviewActor && PreviewWidget)
    {
        PreviewWidget->SetPreviewActor(PreviewActor);
    }
}

void UCharacterSelectionWidget::UpdateCharacterPreview(USkeletalMesh* NewMesh)
{
	if (PreviewActor)
	{
		PreviewActor->SetMesh(NewMesh);
	}
}

void UCharacterSelectionWidget::OnSelectCharacter1()
{
    SelectedCharacterClass = CharacterClass1;
    UpdatePreviewActor(CharacterClass1);

    if (PreviewActor)
    {
        PreviewActor->SetMesh(Character1Mesh);
    }

    HighlightSelectedCharacter(Border_Character1);
}

void UCharacterSelectionWidget::OnSelectCharacter2()
{
    SelectedCharacterClass = CharacterClass2;
    UpdatePreviewActor(CharacterClass2);

    if (PreviewActor)
    {
        PreviewActor->SetMesh(Character2Mesh);
    }

    HighlightSelectedCharacter(Border_Character2);
}

void UCharacterSelectionWidget::OnSelectCharacter3()
{
    SelectedCharacterClass = CharacterClass3;
    UpdatePreviewActor(CharacterClass3);

    if (PreviewActor)
    {
        PreviewActor->SetMesh(Character3Mesh);
    }

    HighlightSelectedCharacter(Border_Character3);
}

void UCharacterSelectionWidget::UpdatePreviewActor(TSubclassOf<ACharacter> CharacterClass)
{
    if (PreviewActor)
    {
        PreviewActor->SetCharacterToDisplay(CharacterClass);
    }
}

void UCharacterSelectionWidget::OnEnterWorld()
{
    if (UYuveaGameInstance* GI = Cast<UYuveaGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->SelectedCharacterClass = SelectedCharacterClass;
    }

    RemoveFromParent();

    if (AYuveaGameMode* GM = Cast<AYuveaGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->StartGameFromMenu();
    }
}


void UCharacterSelectionWidget::OnQuitGame()
{
    if (PreviewActor && !PreviewActor->IsPendingKillPending())
    {
        PreviewActor->Destroy();
        PreviewActor = nullptr;
    }
    
    RemoveFromParent();

    if (!MainMenuWidgetClass) return;

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        UMainMenuWidget* MainMenu = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass);
        if (MainMenu)
        {
            MainMenu->AddToViewport();

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}

void UCharacterSelectionWidget::HighlightSelectedCharacter(UBorder* SelectedBorder)
{
    const FLinearColor SelectedColor = FLinearColor(0.1f, 0.6f, 1.0f, 1.0f);
    const FLinearColor UnselectedColor = FLinearColor(0.05f, 0.05f, 0.05f, 1.0f);

    Border_Character1->SetBrushColor(UnselectedColor);
    Border_Character2->SetBrushColor(UnselectedColor);
    Border_Character3->SetBrushColor(UnselectedColor);

    if (SelectedBorder)
    {
        SelectedBorder->SetBrushColor(SelectedColor);
    }
}
