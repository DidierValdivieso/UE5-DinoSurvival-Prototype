#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include "CharacterPreviewWidget.generated.h"

class USceneCaptureComponent2D;
class ACharacterPreviewActor;

UCLASS()
class THE_YUVEA_PROJECT_API UCharacterPreviewWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetPreviewActor(ACharacterPreviewActor* InPreviewActor);

	void UpdatePreviewMesh(USkeletalMesh* NewMesh);

	UPROPERTY(BlueprintReadOnly)
	FString CharacterID;
	
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPreviewImage;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY()
	ACharacterPreviewActor* SpawnedCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	TSubclassOf<ACharacterPreviewActor> PreviewActorClass;

protected:
	virtual void NativeConstruct() override;
};
