#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GliderAnimInstance.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API UGliderAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UGliderAnimInstance();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    FVector2D InputData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsInAir;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsJumpingStart;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsJumpingEnd;

    void NotifyJumpPressed();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsInWater;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsSwimming;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsFlying;

protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    float JumpStartTimer;
    bool bJumpStartTimerActive;
};
