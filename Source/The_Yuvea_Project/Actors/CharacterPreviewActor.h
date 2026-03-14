#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameFramework/SpringArmComponent.h>
#include "CharacterPreviewActor.generated.h"

class USceneCaptureComponent2D;
class USkeletalMeshComponent;
class UTextureRenderTarget2D;

UCLASS()
class THE_YUVEA_PROJECT_API ACharacterPreviewActor : public AActor
{
	GENERATED_BODY()

public:
	ACharacterPreviewActor();

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(VisibleAnywhere)
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditDefaultsOnly, Category = "Render")
	UTextureRenderTarget2D* RT_RenderTarget;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootScene;

	void SetMesh(USkeletalMesh* Mesh);

	void SetCharacterToDisplay(TSubclassOf<ACharacter> CharacterClass);

	UTextureRenderTarget2D* GetRenderTarget() const { return RT_RenderTarget; }
};