#include "SmallDinosaurCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "../Animators/SmallDinosaurAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <Kismet/KismetMathLibrary.h>

ASmallDinosaurCharacter::ASmallDinosaurCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DinoMesh(TEXT("/Game/StylizedCreaturesBundle/Meshes/SmallDinosaur/SK_SmallDinosaur"));
	if (DinoMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(DinoMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> DinoAnimBP(TEXT("/Game/Blueprints/ABP_SmallDinosaur"));
	if (DinoAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(DinoAnimBP.Class);
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

void ASmallDinosaurCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASmallDinosaurCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASmallDinosaurCharacter::BeginPlay()
{
	Super::BeginPlay();
}