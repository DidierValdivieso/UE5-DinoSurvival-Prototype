#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BiterCharacter.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API ABiterCharacter : public AMainCharacter
{
	GENERATED_BODY()

public:
	ABiterCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;

protected:
	virtual void Tick(float DeltaTime) override;
};
