#include "YuveaGameMode.h"
#include "Widgets/GameStatsWidget.h"
#include "Characters/SmallDinosaurCharacter.h"
#include "Controllers/MainPlayerController.h"
#include "Characters/BatCharacter.h"
#include "Sound/SoundBase.h"
#include <Kismet/GameplayStatics.h>
#include "YuveaGameInstance.h"
#include "GameFramework/PlayerStart.h"

AYuveaGameMode::AYuveaGameMode()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("/Game/Blueprints/Widgets/WBP_GameStatsWidget"));
    if (WidgetFinder.Succeeded())
    {
        GameStatsWidgetClass = WidgetFinder.Class;
    }

    PlayerControllerClass = AMainPlayerController::StaticClass();

    static ConstructorHelpers::FObjectFinder<USoundBase> MenuSoundFinder(TEXT("/Game/Sounds/Yuvea_Main_Menu"));
    if (MenuSoundFinder.Succeeded())
    {
        OpenMenuSound = MenuSoundFinder.Object;
    }
}

void AYuveaGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AYuveaGameMode::SetupHUD()
{
    if (!GameStatsWidgetClass) return;

    GameStatsWidgetInstance = CreateWidget<UGameStatsWidget>(GetWorld(), GameStatsWidgetClass);
    if (!GameStatsWidgetInstance) return;

    GameStatsWidgetInstance->AddToViewport();

    APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);

    if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(Pawn))
    {
        GameStatsWidgetInstance->SetOwnerCharacter(MainCharacter);
        MainCharacter->SetStatsWidget(GameStatsWidgetInstance);
    }
}

void AYuveaGameMode::StartGameFromMenu()
{
    SpawnSelectedCharacter();
    SetupHUD();

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;

        if (APawn* Pawn = PC->GetPawn())
        {
            Pawn->EnableInput(PC);
        }
    }
}

void AYuveaGameMode::SpawnSelectedCharacter()
{
    UYuveaGameInstance* GI = Cast<UYuveaGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GI && GI->SelectedCharacterClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            if (PC->GetPawn())
            {
                PC->GetPawn()->Destroy();
            }

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            AActor* StartSpot = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());
            FVector SpawnLocation = StartSpot ? StartSpot->GetActorLocation() : FVector(0.f, 0.f, 200.f);
            FRotator SpawnRotation = StartSpot ? StartSpot->GetActorRotation() : FRotator::ZeroRotator;

            ACharacter* NewCharacter = GetWorld()->SpawnActor<ACharacter>(GI->SelectedCharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
            if (NewCharacter)
            {
                PC->Possess(NewCharacter);
            }
        }
    }
}

void AYuveaGameMode::StartGame()
{
    UYuveaGameInstance* GI = Cast<UYuveaGameInstance>(GetGameInstance());
    SpawnSelectedCharacter();
    SetupHUD();

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    PC->SetInputMode(FInputModeGameOnly());
    PC->bShowMouseCursor = false;
}