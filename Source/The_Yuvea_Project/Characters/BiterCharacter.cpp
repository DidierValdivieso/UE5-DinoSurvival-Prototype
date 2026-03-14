#include "BiterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "Animation/AnimInstance.h"
#include <Kismet/KismetMathLibrary.h>

ABiterCharacter::ABiterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BiterMesh(TEXT("/Game/Characters/Biter/Biter_SM"));
	if (BiterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BiterMesh.Object);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> BiterAnimBP(TEXT("/Game/Blueprints/Animations/Biter/ABP_Biter"));
	if (BiterAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BiterAnimBP.Class);
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

	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->GravityScale = 2.0f;
}

void ABiterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}