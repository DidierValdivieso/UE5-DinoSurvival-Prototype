#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../Widgets/GameStatsWidget.h"
#include <The_Yuvea_Project/Animators/SmallDinosaurAnimInstance.h>
#include "Engine/Engine.h"
#include "../Animators/WatcherAnimInstance.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <Kismet/KismetMathLibrary.h>

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsRunning = false;

	WaterOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterOverlapBox"));
	WaterOverlapBox->SetupAttachment(RootComponent);
	WaterOverlapBox->SetBoxExtent(FVector(50.f, 50.f, 100.f));
	WaterOverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WaterOverlapBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	WaterOverlapBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WaterOverlapBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Controller) return;

	HandleMovement(DeltaTime);

	if (bIsSwimming)
	{
		HandleSwimmingVerticalMovement(DeltaTime);
		HandleSwimmingForwardMovement(DeltaTime);
		HandleSwimmingHorizontalMovement(DeltaTime);
	}

	if (bIsFlying)
	{
		UpdateMinFlyHeight();

		if (VerticalFlightInput != 0.0f)
		{
			const FVector CurrentLocation = GetActorLocation();
			const float DeltaZ = VerticalFlightSpeed * DeltaTime * VerticalFlightInput;
			const float NewZ = FMath::Clamp(CurrentLocation.Z + DeltaZ, MinFlyHeight, MaxFlyHeight);
			SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewZ));
		}

		FHitResult HitResult;
		FVector Start = GetActorLocation();
		FVector End = Start - FVector(0, 0, 150.0f);

		FCollisionQueryParams TraceParams(FName(TEXT("FlightToGroundTrace")), false, this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

		if (bHit && HitResult.Distance < 100.f)
		{
			bIsFlying = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GetCharacterMovement()->GravityScale = 2.0f;
		}
	}

	HandleClimbDebug();
	HandleClimbAttach();
	HandleClimbingLock();
	UpdateInputDirection();
	DecreaseHydration(DeltaTime * 0.2f);
	DecreaseEnergy(DeltaTime * 0.2f);

	if (CurrentHydration <= 0.f && Energy <= 0.f)
	{
		DecreaseLife(DeltaTime * 2.0f);
	}
	else if (CurrentHydration >= 50.f && Energy >= 50.f)
	{
		RegenerateLife(DeltaTime * FinalRegenRate);
	}

	if (IsInWater())
	{
		float BreathConsumptionRate = 5.0f * (1.0f + BreathModifier);
		DecreaseBreath(DeltaTime * BreathConsumptionRate);

		if (CurrentBreath <= 0.f)
		{
			DecreaseLife(DeltaTime * 10.f);
		}
	}
	else
	{
		IncreaseBreath(DeltaTime * 10.0f);
	}

	if (SkillPoints < MaxSkillPoints)
	{
		SkillPointTimer += DeltaTime;

		if (SkillPointTimer >= SecondsPerSkillPoint)
		{
			SkillPoints++;
			SkillPointTimer = 0.0f;
		}
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxisKey(EKeys::MouseX, this, &AMainCharacter::OnMouseX);
	PlayerInputComponent->BindAxisKey(EKeys::MouseY, this, &AMainCharacter::OnMouseY);
	PlayerInputComponent->BindKey(EKeys::W, IE_Pressed, this, &AMainCharacter::HandleMoveForwardPressed);
	PlayerInputComponent->BindKey(EKeys::W, IE_Released, this, &AMainCharacter::HandleMoveForwardReleased);
	PlayerInputComponent->BindKey(EKeys::S, IE_Pressed, this, &AMainCharacter::HandleMoveBackwardPressed);
	PlayerInputComponent->BindKey(EKeys::S, IE_Released, this, &AMainCharacter::HandleMoveBackwardReleased);
	PlayerInputComponent->BindKey(EKeys::D, IE_Pressed, this, &AMainCharacter::HandleMoveRightPressed);
	PlayerInputComponent->BindKey(EKeys::D, IE_Released, this, &AMainCharacter::HandleMoveRightReleased);
	PlayerInputComponent->BindKey(EKeys::A, IE_Pressed, this, &AMainCharacter::HandleMoveLeftPressed);
	PlayerInputComponent->BindKey(EKeys::A, IE_Released, this, &AMainCharacter::HandleMoveLeftReleased);
	PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this, &AMainCharacter::TraceForClimbTarget);
	PlayerInputComponent->BindKey(EKeys::F, IE_Released, this, &AMainCharacter::JumpToClimbPoint);
	PlayerInputComponent->BindKey(EKeys::LeftAlt, IE_Pressed, this, &AMainCharacter::DetachFromClimb);
	PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &AMainCharacter::HandleJump);
	PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Released, this, &AMainCharacter::StopAscend);
	PlayerInputComponent->BindKey(EKeys::LeftControl, IE_Pressed, this, &AMainCharacter::HandleDescendPressed);
	PlayerInputComponent->BindKey(EKeys::LeftControl, IE_Released, this, &AMainCharacter::HandleDescendReleased);
	PlayerInputComponent->BindKey(EKeys::E, IE_Released, this, &AMainCharacter::TryDrink);
	PlayerInputComponent->BindKey(EKeys::E, IE_Released, this, &AMainCharacter::Eat);
	PlayerInputComponent->BindKey(EKeys::E, IE_Released, this, &AMainCharacter::EatRotten);
	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Pressed, this, &AMainCharacter::StartRunning);
	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Released, this, &AMainCharacter::StopRunning);
}

void AMainCharacter::UpdateInputDirection()
{
	const FRotator CameraRot = Controller->GetControlRotation();
	const FVector Forward = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::Y);

	FVector InputDirection = Forward * MovementInput.X + Right * MovementInput.Y;

	if (!InputDirection.IsNearlyZero())
	{
		InputDirection.Normalize();
	}

	float CurrentSpeed = GetVelocity().Size();
	FVector LocalVelocity = GetActorTransform().InverseTransformVectorNoScale(InputDirection * CurrentSpeed);
	InputData = FVector2D(LocalVelocity.Y, LocalVelocity.X);
}

void AMainCharacter::OnMouseX(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::OnMouseY(float Value)
{
	AddControllerPitchInput(-Value);
}

void AMainCharacter::StartRunning()
{
	bIsRunning = true;
	ApplySpeedModifiers();
}

void AMainCharacter::StopRunning()
{
	bIsRunning = false;
	ApplySpeedModifiers();
}

void AMainCharacter::TryDrink()
{
	if (!bCanDrink)
		return;

	if (GEngine)
	{
		IncreaseHydration(20.f);
	}
}

bool AMainCharacter::IsInWater() const
{
	return bIsInWater;
}

void AMainCharacter::SetIsInWater(bool bNewState)
{
	bIsInWater = bNewState;
	bIsSwimming = bNewState;

	if (bIsInWater)
	{
		if (StatsWidget && !bCanDrink)
		{
			StatsWidget->ShowWarningMessage(TEXT("Press E to drink water!"));
		}
		bCanDrink = true;

		GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
		GetCharacterMovement()->GravityScale = 0.0f;
	}
	else
	{
		if (StatsWidget)
		{
			StatsWidget->HideWarningMessage();
		}
		bCanDrink = false;

		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->GravityScale = 2.0f;
	}
}

void AMainCharacter::SetIsNearFoodZone(bool bNewState)
{
	bIsNearFoodZone = bNewState;

	if (bIsNearFoodZone)
	{
		if (StatsWidget && !bHasShownEatPrompt)
		{
			StatsWidget->ShowWarningMessage(TEXT("Press E to eat food!"));
		}
		bHasShownEatPrompt = true;
	}
	else
	{
		if (StatsWidget)
		{
			StatsWidget->HideWarningMessage();
		}
		bHasShownEatPrompt = false;
	}
}

void AMainCharacter::SetIsNearFoodZoneRotten(bool bNewState)
{
	bIsNearFoodZoneRotten = bNewState;

	if (bIsNearFoodZoneRotten)
	{
		if (StatsWidget && !bHasShownEatPromptRotten)
		{
			StatsWidget->ShowWarningMessage(TEXT("Press E to eat food! (Risk)"));
		}
		bHasShownEatPromptRotten = true;
	}
	else
	{
		if (StatsWidget)
		{
			StatsWidget->HideWarningMessage();
		}
		bHasShownEatPromptRotten = false;
	}
}

void AMainCharacter::EatRotten()
{
	if (!bIsNearFoodZoneRotten)
		return;

	bool bWasZero = CurrentLife <= 0.f;
	CurrentLife = FMath::Clamp(CurrentLife - (20.f - ResistanceFoodPoisoning) , 0.f, 100.f);

	if (bWasZero && CurrentLife > 0.f)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

float AMainCharacter::GetHydrationRatio() const
{
	return FMath::Clamp(CurrentHydration / MaxHydration, 0.f, 1.f);
}

void AMainCharacter::DecreaseHydration(float Amount)
{
	CurrentHydration = FMath::Clamp(CurrentHydration - Amount, 0.f, MaxHydration);
}

void AMainCharacter::IncreaseHydration(float Amount)
{
	const bool bWasZero = CurrentHydration <= 0.f;
	CurrentHydration = FMath::Clamp(CurrentHydration + Amount, 0.f, MaxHydration);

	if (bWasZero && CurrentHydration > 0.f)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
	}
}

void AMainCharacter::Eat()
{
	if (!bIsNearFoodZone)
		return;

	bool bWasZero = Energy <= 0.f;
	Energy = FMath::Clamp(Energy + 25.f, 0.f, 100.f);

	if (bWasZero && Energy > 0.f)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

float AMainCharacter::GetEnergyRatio() const
{
	return FMath::Clamp(Energy / MaxEnergy, 0.f, 1.f);
}

void AMainCharacter::DecreaseEnergy(float Amount)
{
	Energy = FMath::Clamp(Energy - Amount, 0.f, MaxEnergy);
}

void AMainCharacter::DecreaseLife(float Amount)
{
	CurrentLife = FMath::Clamp(CurrentLife - Amount, 0.f, BaseMaxLife);

	if (CurrentLife <= 0.f)
	{
		GetCharacterMovement()->DisableMovement();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("You died!"));
		}
	}
}

void AMainCharacter::RegenerateLife(float Amount)
{
	if (CurrentLife < BaseMaxLife)
	{
		CurrentLife = FMath::Clamp(CurrentLife + Amount, 0.f, BaseMaxLife);
	}
}

float AMainCharacter::GetBreathRatio() const
{
	return FMath::Clamp(CurrentBreath / BaseMaxBreath, 0.f, 1.f);
}

void AMainCharacter::DecreaseBreath(float Amount)
{
	CurrentBreath = FMath::Clamp(CurrentBreath - Amount, 0.f, BaseMaxBreath);
}

void AMainCharacter::IncreaseBreath(float Amount)
{
	CurrentBreath = FMath::Clamp(CurrentBreath + Amount, 0.f, BaseMaxBreath);
}

void AMainCharacter::ApplySpeedModifiers()
{
	float BaseWalk = WalkSpeed;
	float BaseRun = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ?
		BaseRun * (1.0f + SpeedModifier) :
		BaseWalk * (1.0f + SpeedModifier);
}

void AMainCharacter::ApplyLifeModifiers()
{
	BaseMaxLife = BaseMaxLife * (1.0f + BonusMaxLifePercent / 100.0f);
	CurrentLife = FMath::Min(CurrentLife, BaseMaxLife);
}

void AMainCharacter::ApplyBreathModifiers()
{
	BaseMaxBreath = BaseMaxBreath + BonusBreathSeconds;
}

void AMainCharacter::ApplyBreathModifierEffect()
{
	float BreathConsumptionRate = 5.0f * (1.0f + BreathModifier);
}

void AMainCharacter::ApplyResistanceFoodPoisoningEffect()
{
	float BreathConsumptionRate = (ResistanceFoodPoisoning);
}

void AMainCharacter::ApplyRegenModifiers()
{
	FinalRegenRate = BaseRegenRate * (1.0f + BonusRegenPercent / 100.0f);
}

void AMainCharacter::HandleMoveForwardPressed()
{
	if (bIsSwimming)
		bSwimForwardInput = true;
	else
		bMoveForwardInput = true;
}

void AMainCharacter::HandleMoveForwardReleased()
{
	if (bIsSwimming)
		bSwimForwardInput = false;
	else
		bMoveForwardInput = false;
}

void AMainCharacter::HandleMoveBackwardPressed()
{
	if (bIsSwimming)
		bSwimBackwardInput = true;
	else
		bMoveBackwardInput = true;
}

void AMainCharacter::HandleMoveBackwardReleased()
{
	if (bIsSwimming)
		bSwimBackwardInput = false;
	else
		bMoveBackwardInput = false;
}

void AMainCharacter::HandleMoveRightPressed()
{
	if (bIsSwimming)
		bSwimRightInput = true;
	else
		bMoveRightInput = true;
}

void AMainCharacter::HandleMoveRightReleased()
{
	if (bIsSwimming)
		bSwimRightInput = false;
	else
		bMoveRightInput = false;
}

void AMainCharacter::HandleMoveLeftPressed()
{
	if (bIsSwimming)
		bSwimLeftInput = true;
	else
		bMoveLeftInput = true;
}

void AMainCharacter::HandleMoveLeftReleased()
{
	if (bIsSwimming)
		bSwimLeftInput = false;
	else
		bMoveLeftInput = false;
}

void AMainCharacter::HandleDescendPressed()
{
	if (bIsFlying)
	{
		VerticalFlightInput = -1.0f;
	}
	else if (bIsSwimming)
	{
		bSwimDownInput = true;
	}
}

void AMainCharacter::HandleDescendReleased()
{
	if (bIsFlying)
	{
		VerticalFlightInput = 0.0f;
	}
	else if (bIsSwimming)
	{
		bSwimDownInput = false;
	}
}

void AMainCharacter::HandleSwimmingHorizontalMovement(float DeltaTime)
{
	if (!bIsSwimming)
		return;

	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector NewVelocity = FVector::ZeroVector;

	if (bSwimForwardInput)
	{
		NewVelocity += ForwardVector * SwimForwardSpeed;
	}
	else if (bSwimBackwardInput)
	{
		NewVelocity -= ForwardVector * SwimBackwardSpeed;
	}

	if (bSwimRightInput)
	{
		NewVelocity += RightVector * SwimStrafeSpeed;
	}
	else if (bSwimLeftInput)
	{
		NewVelocity -= RightVector * SwimStrafeSpeed;
	}

	NewVelocity.Z = GetCharacterMovement()->Velocity.Z;

	GetCharacterMovement()->Velocity = NewVelocity;
}

void AMainCharacter::HandleSwimmingStrafeMovement(float DeltaTime)
{
	if (!bIsSwimming)
		return;

	FVector RightVector = GetActorRightVector();
	FVector Velocity = GetCharacterMovement()->Velocity;

	if (bSwimRightInput)
	{
		Velocity += RightVector * SwimStrafeSpeed;
	}
	else if (bSwimLeftInput)
	{
		Velocity -= RightVector * SwimStrafeSpeed;
	}
	else
	{
		Velocity.X = 0.f;
		Velocity.Y = 0.f;
	}

	GetCharacterMovement()->Velocity = Velocity;
}

void AMainCharacter::HandleSwimmingForwardMovement(float DeltaTime)
{
	if (!bIsSwimming)
		return;

	FVector ForwardVector = GetActorForwardVector();
	FVector Velocity = GetCharacterMovement()->Velocity;

	if (bSwimForwardInput)
	{
		Velocity.X = ForwardVector.X * SwimForwardSpeed;
		Velocity.Y = ForwardVector.Y * SwimForwardSpeed;
	}
	else if (bSwimBackwardInput)
	{
		Velocity.X = ForwardVector.X * -SwimBackwardSpeed;
		Velocity.Y = ForwardVector.Y * -SwimBackwardSpeed;
	}
	else
	{
		Velocity.X = 0.f;
		Velocity.Y = 0.f;
	}

	GetCharacterMovement()->Velocity = Velocity;
}

void AMainCharacter::HandleClimbDebug()
{
	if (bHasClimbTarget)
	{
		DrawDebugSphere(GetWorld(), TargetClimbPoint, 25.f, 12, FColor::Red, false, -1.f, 0);
	}
}

void AMainCharacter::HandleClimbingLock()
{
	if (bIsClimbing)
	{
		SetActorLocation(ClimbAttachLocation);
	}
}

void AMainCharacter::HandleClimbAttach()
{
	if (bWantsToAttach && !bIsClimbing)
	{
		float Distance = FVector::Dist(GetActorLocation(), DesiredAttachPoint);
		if (Distance < 100.f && GetCharacterMovement()->IsFalling())
		{
			FVector Offset = -LastClimbLaunchDir * 40.f;
			FVector FinalAttachLocation = DesiredAttachPoint + Offset;

			SetActorLocation(FinalAttachLocation);
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->DisableMovement();
			GetCharacterMovement()->GravityScale = 0.f;

			ClimbAttachLocation = FinalAttachLocation;
			bIsClimbing = true;
			bWantsToAttach = false;

			FRotator LookAtRotation = LastClimbLaunchDir.ToOrientationRotator();
			SetActorRotation(LookAtRotation);
		}
	}
}

void AMainCharacter::JumpToClimbPoint()
{
	if (bHasClimbTarget)
	{
		if (bIsClimbing)
		{
			DetachFromClimb();
		}

		bWantsToAttach = true;
		DesiredAttachPoint = TargetClimbPoint;

		LastClimbLaunchDir = (DesiredAttachPoint - GetActorLocation()).GetSafeNormal();
		FVector LaunchDir = LastClimbLaunchDir;

		float Distance = FVector::Dist(DesiredAttachPoint, GetActorLocation());
		float LaunchStrength = FMath::Clamp(Distance * 2.f, 800.f, 2000.f);

		LaunchCharacter(LaunchDir * LaunchStrength + FVector(0, 0, 500.f), true, true);
		bHasClimbTarget = false;
	}
}

void AMainCharacter::TryClimbAttach()
{
	if (!bWantsToAttach) return;

	float Distance = FVector::Dist(GetActorLocation(), TargetClimbPoint);

	if (Distance < 120.f)
	{
		FVector Offset = -LastClimbLaunchDir * 40.f;
		FVector FinalAttachLocation = TargetClimbPoint + Offset;

		SetActorLocation(FinalAttachLocation);

		FRotator FinalRotation = UKismetMathLibrary::MakeRotFromXZ(-ClimbSurfaceNormal, GetActorUpVector());
		SetActorRotation(FinalRotation);

		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->GravityScale = 0.f;

		ClimbAttachLocation = FinalAttachLocation;
		bIsClimbing = true;
		bWantsToAttach = false;
	}
}

void AMainCharacter::DetachFromClimb()
{
	if (bIsClimbing)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetCharacterMovement()->GravityScale = 2.f;
		GetCharacterMovement()->SetDefaultMovementMode();

		bIsClimbing = false;
		ClimbTargetActor = nullptr;
	}
}

void AMainCharacter::TraceForClimbTarget()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	int32 ViewportSizeX, ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

	float ScreenX = ViewportSizeX / 2.f;
	float ScreenY = ViewportSizeY / 2.f;

	FVector WorldLocation;
	FVector WorldDirection;

	if (PC->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection))
	{
		FVector Start = GetActorLocation() + FVector(0, 0, 50.f);
		FVector End = Start + WorldDirection * 2000.f;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			if (Hit.Component.IsValid() && Hit.Component->IsA<UInstancedStaticMeshComponent>())
			{
				TargetClimbPoint = Hit.Location;
				bHasClimbTarget = true;
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0, 2.0f);
		}
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainCharacter::HandleSwimmingVerticalMovement(float DeltaTime)
{
	if (!bIsSwimming)
		return;

	FVector Velocity = GetCharacterMovement()->Velocity;

	if (bSwimUpInput)
	{
		Velocity.Z = SwimUpSpeed;
	}
	else if (bSwimDownInput)
	{
		Velocity.Z = -SwimDownSpeed;
	}
	else
	{
		Velocity.Z = 0.0f;
	}

	GetCharacterMovement()->Velocity = Velocity;
}

void AMainCharacter::Jump()
{
	if (bIsFlying)
		return;

	if (!GetCharacterMovement()->IsFalling())
	{
		Super::Jump();

		if (UWatcherAnimInstance* WatcherAnim = Cast<UWatcherAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			WatcherAnim->NotifyJumpPressed();
		}
	}
}

void AMainCharacter::HandleJump()
{
	if (bIsFlying)
	{
		VerticalFlightInput = 1.0f;
		return;
	}

	if (bIsSwimming)
	{
		bSwimUpInput = true;
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (!GetCharacterMovement()->IsFalling())
	{
		JumpCount = 1;
		LastJumpTime = CurrentTime;
		Jump();
	}
	else if (JumpCount == 1 && (CurrentTime - LastJumpTime) <= MaxDoubleTapDelay)
	{
		StartFlyUp();
		JumpCount = 0;
	}
	else
	{
		JumpCount = 0;
	}
}

void AMainCharacter::StopAscend()
{
	if (bIsFlying)
	{
		VerticalFlightInput = 0.0f;
	}
	else if (bIsSwimming)
	{
		bSwimUpInput = false;
	}
}

void AMainCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	JumpCount = 0;
}

void AMainCharacter::UpdateMinFlyHeight()
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 10000);

	FCollisionQueryParams TraceParams(FName(TEXT("GroundTrace")), false, this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		TraceParams
	);

	if (bHit)
	{
		MinFlyHeight = HitResult.ImpactPoint.Z + 50.0f;
	}
}

void AMainCharacter::HandleMovement(float DeltaTime)
{
	MovementInput = FVector2D::ZeroVector;

	if (bMoveForwardInput)
		MovementInput.X += 1.0f;
	if (bMoveBackwardInput)
		MovementInput.X -= 1.0f;
	if (bMoveRightInput)
		MovementInput.Y += 1.0f;
	if (bMoveLeftInput)
		MovementInput.Y -= 1.0f;

	if (!MovementInput.IsNearlyZero())
	{
		const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		if (bIsFlying)
		{
			FVector NewVelocity = (Forward * MovementInput.X + Right * MovementInput.Y) * FlySpeed;
			NewVelocity.Z = GetCharacterMovement()->Velocity.Z;
			GetCharacterMovement()->Velocity = NewVelocity;
		}
		else
		{
			AddMovementInput(Forward, MovementInput.X);
			AddMovementInput(Right, MovementInput.Y);
		}

		FRotator TargetRotation = Controller->GetControlRotation();
		TargetRotation.Pitch = 0.f;
		TargetRotation.Roll = 0.f;

		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 12.0f);
		SetActorRotation(NewRotation);
	}
	else if (bIsFlying)
	{
		FVector Velocity = GetCharacterMovement()->Velocity;
		Velocity.X = 0.0f;
		Velocity.Y = 0.0f;
		GetCharacterMovement()->Velocity = Velocity;
	}
}

void AMainCharacter::StartFlyUp()
{
	if (bIsFlying) return;

	FVector Velocity = GetCharacterMovement()->Velocity;
	Velocity.Z = 0.0f;
	GetCharacterMovement()->Velocity = Velocity;

	bIsFlying = true;
	VerticalFlightInput = 1.0f;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.0f;
}

void AMainCharacter::StartFlyDown()
{
	bIsFlying = true;
	VerticalFlightInput = -1.0f;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.0f;
}

void AMainCharacter::StopFlyVertical()
{
	VerticalFlightInput = 0.0f;
}