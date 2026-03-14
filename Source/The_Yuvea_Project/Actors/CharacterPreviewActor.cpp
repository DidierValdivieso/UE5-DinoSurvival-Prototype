#include "CharacterPreviewActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include <GameFramework/SpringArmComponent.h>
#include "GameFramework/Character.h"

ACharacterPreviewActor::ACharacterPreviewActor()
{
    PrimaryActorTick.bCanEverTick = false;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMesh"));
    PreviewMesh->SetupAttachment(RootComponent);
    PreviewMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    PreviewMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
    PreviewMesh->SetRelativeScale3D(FVector(30.f));

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(RootComponent);
    SceneCapture->SetRelativeLocation(FVector(-400.f, 0.f, 250.f));
    SceneCapture->SetRelativeRotation(FRotator(0.f, -30.f, 0.f));
    SceneCapture->bCaptureEveryFrame = true;
    SceneCapture->bCaptureOnMovement = false;

    RT_RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    RT_RenderTarget->InitAutoFormat(1920, 1080);
    RT_RenderTarget->UpdateResourceImmediate(true);

    SceneCapture->TextureTarget = RT_RenderTarget;
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
}

void ACharacterPreviewActor::SetMesh(USkeletalMesh* Mesh)
{
    if (!Mesh || !PreviewMesh)
    {
        return;
    }

    PreviewMesh->SetSkeletalMesh(Mesh);

    if (SceneCapture && SceneCapture->TextureTarget)
    {
        SceneCapture->CaptureScene();
    }
}

void ACharacterPreviewActor::SetCharacterToDisplay(TSubclassOf<ACharacter> CharacterClass)
{
    if (!CharacterClass) return;

    ACharacter* DefaultChar = CharacterClass->GetDefaultObject<ACharacter>();
    if (!DefaultChar) return;

    USkeletalMeshComponent* DefaultMesh = DefaultChar->GetMesh();
    if (!DefaultMesh || !DefaultMesh->SkeletalMesh) return;

    PreviewMesh->SetSkeletalMesh(DefaultMesh->SkeletalMesh);
}

