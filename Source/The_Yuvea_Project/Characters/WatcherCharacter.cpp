#include "WatcherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "Animation/AnimInstance.h"
#include <Kismet/KismetMathLibrary.h>

AWatcherCharacter::AWatcherCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WatcherMesh(TEXT("/Game/Characters/Watcher/Blep_Watcher_SM"));
	if (WatcherMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(WatcherMesh.Object);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> WatcherAnimBP(TEXT("/Game/Blueprints/ABP_Watcher"));
	if (WatcherAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WatcherAnimBP.Class);
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

void AWatcherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}