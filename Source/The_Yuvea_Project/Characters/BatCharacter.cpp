#include "BatCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Animators/BatAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"

ABatCharacter::ABatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BatMesh(TEXT("/Game/StylizedCreaturesBundle/Meshes/Bat/SK_Bat"));
	if (BatMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BatMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BatAnimBP(TEXT("/Game/Blueprints/Animations/ABP_Bat"));
	if (BatAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BatAnimBP.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");

	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 150.0f);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
}

void ABatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABatCharacter::BeginPlay()
{
	Super::BeginPlay();
}
