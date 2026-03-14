#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../Widgets/SkillTreeWidget.h"
#include "../Widgets/MainMenuWidget.h"
#include "../Widgets/CharacterPreviewWidget.h"
#include "../Widgets/CharacterSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "../YuveaGameMode.h"

AMainPlayerController::AMainPlayerController()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> SkillFinder(TEXT("/Game/Blueprints/Widgets/WBP_SkillTreeWidget"));
    if (SkillFinder.Succeeded())
    {
        SkillTreeWidgetClass = SkillFinder.Class;
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> MenuFinder(TEXT("/Game/Blueprints/Widgets/WBP_MainMenuWidget"));
    if (MenuFinder.Succeeded())
    {
        MainMenuWidgetClass = MenuFinder.Class;
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> SettingsFinder(TEXT("/Game/Blueprints/Widgets/WBP_SettingsWidget"));
    if (SettingsFinder.Succeeded())
    {
        SettingsWidgetClass = SettingsFinder.Class;
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> PreviewFinder(TEXT("/Game/Blueprints/Widgets/WBP_CharacterPreviewWidget"));
    if (PreviewFinder.Succeeded())
    {
        CharacterPreviewWidgetClass = PreviewFinder.Class;
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> CharSelectFinder(TEXT("/Game/Blueprints/Widgets/WBP_CharacterSelectionWidget"));
    if (CharSelectFinder.Succeeded())
    {
        CharacterSelectionWidgetClass = CharSelectFinder.Class;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> MenuSoundObj(TEXT("/Game/Sounds/Yuvea_Main_Menu"));
    if (MenuSoundObj.Succeeded())
    {
        MenuMusic = MenuSoundObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> GameplaySoundObj(TEXT("/Game/Sounds/Yuvea_s_Atomspheric_Music"));
    if (GameplaySoundObj.Succeeded())
    {
        GameplayMusic = GameplaySoundObj.Object;
    }
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ShowMainMenu();    
}

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("ToggleSkillTree", IE_Pressed, this, &AMainPlayerController::ToggleSkillTree);
    InputComponent->BindAction("ToggleSettings", IE_Pressed, this, &AMainPlayerController::ToggleSettingsMenu);
}

void AMainPlayerController::ToggleSettingsMenu()
{
    if (!SettingsWidgetInstance && SettingsWidgetClass)
    {
        SettingsWidgetInstance = CreateWidget<UUserWidget>(this, SettingsWidgetClass);
    }

    if (!SettingsWidgetInstance) return;

    if (!bIsSettingsOpen)
    {
        SettingsWidgetInstance->AddToViewport();

        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(SettingsWidgetInstance->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);

        bShowMouseCursor = true;
        bIsSettingsOpen = true;
    }
    else
    {
        SettingsWidgetInstance->RemoveFromParent();

        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        bShowMouseCursor = false;
        bIsSettingsOpen = false;
    }
}


void AMainPlayerController::ToggleSkillTree()
{
    if (!SkillTreeWidget && SkillTreeWidgetClass)
    {
        SkillTreeWidget = CreateWidget<USkillTreeWidget>(this, SkillTreeWidgetClass);
    }

    if (!SkillTreeWidget) return;

    if (!SkillTreeWidget->IsInViewport())
    {
        SkillTreeWidget->AddToViewport();

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(SkillTreeWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);
        bShowMouseCursor = true;
        bIsSkillTreeVisible = true;
    }
    else
    {
        SkillTreeWidget->RemoveFromParent();
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
        bIsSkillTreeVisible = false;
    }
}

void AMainPlayerController::ShowMainMenu()
{
    MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
    
    if (!MainMenuWidget) return;

    MainMenuWidget->OnSinglePlayerClickedEvent.AddDynamic(this, &AMainPlayerController::OnSinglePlayerStart);

    if (!MainMenuWidget->IsInViewport())
    {
        MainMenuWidget->AddToViewport();

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);
        bShowMouseCursor = true;
        bIsSkillTreeVisible = true;
    }
    else
    {
        MainMenuWidget->RemoveFromParent();
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
        bIsSkillTreeVisible = false;
    }

    if (GetPawn())
    {
        GetPawn()->DisableInput(this);
    }

    if (MenuMusic)
    {
        CurrentMusicComponent = UGameplayStatics::SpawnSound2D(this, MenuMusic, 0.6f);
    }

}

void AMainPlayerController::HideMainMenu()
{
    if (MainMenuWidget)
    {
        MainMenuWidget->RemoveFromParent();
        MainMenuWidget = nullptr;
    }

    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;

    if (GetPawn())
    {
        GetPawn()->EnableInput(this);
    }
}

void AMainPlayerController::OnSinglePlayerStart()
{
    if (AYuveaGameMode* GM = Cast<AYuveaGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->SetupHUD();
        SetupGameplayInput();
    }

    if (CurrentMusicComponent)
    {
        CurrentMusicComponent->Stop();
        CurrentMusicComponent = nullptr;
    }

    if (GameplayMusic)
    {
        CurrentMusicComponent = UGameplayStatics::SpawnSound2D(this, GameplayMusic, 0.3f);
    }

    HideMainMenu();
}

void AMainPlayerController::SetupGameplayInput()
{
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);

    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
    bEnableTouchEvents = false;

    if (UGameViewportClient* Viewport = GetWorld()->GetGameViewport())
    {
        Viewport->SetMouseLockMode(EMouseLockMode::LockAlways);
        Viewport->Viewport->CaptureMouse(true);
        Viewport->Viewport->LockMouseToViewport(true);
    }
}
