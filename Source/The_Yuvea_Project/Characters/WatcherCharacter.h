#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WatcherCharacter.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API AWatcherCharacter : public AMainCharacter
{
	GENERATED_BODY()

public:
	AWatcherCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;

protected:
	virtual void Tick(float DeltaTime) override;
};
