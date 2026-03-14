#include "CharacterPreviewWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Character.h" 
#include "Camera/CameraComponent.h"
#include "../Actors/CharacterPreviewActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include <Kismet/GameplayStatics.h>

void UCharacterPreviewWidget::NativeConstruct()
{
    Super::NativeConstruct();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterPreviewActor::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        SetPreviewActor(Cast<ACharacterPreviewActor>(FoundActors[0]));
    }
}

void UCharacterPreviewWidget::SetPreviewActor(ACharacterPreviewActor* InPreviewActor)
{
    if (!InPreviewActor)
    {
        return;
    }

    if (!GetWorld())
    {
        return;
    }

    SpawnedCharacter = InPreviewActor;
    RenderTarget = InPreviewActor->GetRenderTarget();

    if (!RenderTarget)
    {
        return;
    }

    if (!CharacterPreviewImage)
    {
        return;
    }
}

void UCharacterPreviewWidget::UpdatePreviewMesh(USkeletalMesh* NewMesh)
{
    if (SpawnedCharacter)
    {
        SpawnedCharacter->SetMesh(NewMesh);
    }
}