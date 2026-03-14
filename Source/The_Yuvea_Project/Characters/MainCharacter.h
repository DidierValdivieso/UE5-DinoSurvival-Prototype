#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/BoxComponent.h>
#include "MainCharacter.generated.h"

class UGameStatsWidget;

UENUM(BlueprintType)
enum class EFoodType : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rotten UMETA(DisplayName = "Rotten")
};

UCLASS()
class THE_YUVEA_PROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

private:
	void Jump();
	void HandleMovement(float DeltaTime);
	void UpdateInputDirection();

	void HandleMoveForwardPressed();
	void HandleMoveForwardReleased();
	void HandleMoveBackwardPressed();
	void HandleMoveBackwardReleased();
	void HandleMoveRightPressed();
	void HandleMoveRightReleased();
	void HandleMoveLeftPressed();
	void HandleMoveLeftReleased();
	void HandleDescendPressed();
	void HandleDescendReleased();
	void OnMouseX(float Value);
	void OnMouseY(float Value);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 1200.0f;	

	UPROPERTY()
	FVector2D MovementInput;

	bool bForwardPressed = false;
	bool bBackwardPressed = false;
	bool bRightPressed = false;
	bool bLeftPressed = false;
	bool bIsRunning = false;
	

protected:
	void HandleSwimmingVerticalMovement(float DeltaTime);
	void HandleSwimmingHorizontalMovement(float DeltaTime);
	void HandleSwimmingStrafeMovement(float DeltaTime);
	void HandleSwimmingForwardMovement(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swimming")
	float SwimUpSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swimming")
	float SwimDownSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swimming")
	float SwimForwardSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swimming")
	float SwimBackwardSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swimming")
	float SwimStrafeSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsSwimming = false;

	bool bSwimUpInput = false;
	bool bSwimDownInput = false;
	bool bSwimForwardInput = false;
	bool bSwimBackwardInput = false;
	bool bSwimRightInput = false;
	bool bSwimLeftInput = false;

protected:
	void StartFlyUp();
	void StartFlyDown();
	void StopFlyVertical();
	void StopAscend();

	void UpdateMinFlyHeight();

	UPROPERTY(VisibleAnywhere, Category = "Flight")
	bool bIsFlying = false;

	UPROPERTY(VisibleAnywhere, Category = "Flight")
	float MinFlyHeight = -30000.0f;

	UPROPERTY(EditAnywhere, Category = "Flight")
	float MaxFlyHeight = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "Flight")
	float VerticalFlightSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float FlySpeed = 600.0f;

	float WaterSurfaceZ = 300.f;
	float VerticalFlightInput = 0.0f;

	bool bMoveForwardInput = false;
	bool bMoveBackwardInput = false;
	bool bMoveRightInput = false;
	bool bMoveLeftInput = false;

protected:
	void HandleClimbDebug();
	void HandleClimbingLock();
	void HandleClimbAttach();
	void DetachFromClimb();
	void TryClimbAttach();
	void JumpToClimbPoint();
	void TraceForClimbTarget();

	UPROPERTY(BlueprintReadOnly)
	bool bIsClimbing = false;

	UPROPERTY()
	AActor* ClimbTargetActor = nullptr;

	FVector TargetClimbPoint;
	FVector ClimbAttachLocation;
	FVector ClimbSurfaceNormal;
	FVector LastClimbLaunchDir;
	FVector DesiredAttachPoint;

	bool bHasClimbTarget = false;
	bool bWantsToAttach = false;

	FTimerHandle TimerHandle_ClimbCheck;

public:
	float SpeedModifier = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector2D InputData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Energy = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxEnergy = 100.f;

	float GetCurrentEnergy() const { return Energy; }
	float GetMaxEnergy() const { return MaxEnergy; }
	float GetEnergyRatio() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHydration = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHydration = 70.f;

	float GetCurrentHydration() const { return CurrentHydration; }
	float GetMaxHydration() const { return MaxHydration; }
	float GetHydrationRatio() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float BaseMaxLife = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float CurrentLife = 100.f;

	float BonusMaxLifePercent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float BaseMaxBreath = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float CurrentBreath = 100.f;

	float BonusBreathSeconds = 0.f;
	float BreathModifier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float FinalRegenRate = 1.0f;

	float BaseRegenRate = 1.0f;
	float BonusRegenPercent = 0.f;

	float ResistanceFoodPoisoning = 0.0f;

	UFUNCTION(BlueprintCallable)
	float GetBreathRatio() const;

	float GetCurrentBreath() const { return CurrentBreath; }
	float GetMaxBreath() const { return BaseMaxBreath; }

	void DecreaseBreath(float Amount);
	void IncreaseBreath(float Amount);

	void DecreaseHydration(float Amount);
	void IncreaseHydration(float Amount);

	void DecreaseEnergy(float Amount);
	void DecreaseLife(float Amount);
	void RegenerateLife(float Amount);

	void ApplySpeedModifiers();
	void ApplyLifeModifiers();
	void ApplyBreathModifiers();
	void ApplyBreathModifierEffect();
	void ApplyResistanceFoodPoisoningEffect();
	void ApplyRegenModifiers();

	UPROPERTY(EditAnywhere, Category = "SkillPoints")
	int16 SkillPoints = 0;

	UPROPERTY(EditAnywhere, Category = "SkillPoints")
	float SecondsPerSkillPoint = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "SkillPoints")
	int16 MaxSkillPoints = 24;

	float SkillPointTimer = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInWater = false;

	UPROPERTY(BlueprintReadOnly)
	bool bCanDrink = false;

	bool bIsNearFoodZone = false;
	bool bHasShownEatPrompt = false;

	bool bIsNearFoodZoneRotten = false;
	bool bHasShownEatPromptRotten = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food")
	EFoodType CurrentFoodType = EFoodType::Normal;

	FTimerHandle RottenFoodDamageHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Water")
	UBoxComponent* WaterOverlapBox;

	void TryDrink();
	void Eat();
	void EatRotten();

	bool IsInWater() const;
	void SetIsInWater(bool bNewState);
	void SetIsNearFoodZone(bool bNewState);
	void SetIsNearFoodZoneRotten(bool bNewState);

	UFUNCTION()
	void HandleJump();

	UFUNCTION()
	void StartRunning();

	UFUNCTION()
	void StopRunning();

	float LastJumpTime = 0.0f;
	int32 JumpCount = 0;
	float MaxDoubleTapDelay = 0.35f;

	UPROPERTY()
	UGameStatsWidget* StatsWidget;

	void SetStatsWidget(UGameStatsWidget* Widget) { StatsWidget = Widget; }
};
