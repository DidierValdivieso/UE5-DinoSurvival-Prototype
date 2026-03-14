#include "BiterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Characters/BiterCharacter.h"

UBiterAnimInstance::UBiterAnimInstance()
    : Speed(0.f)
    , InputData(FVector2D::ZeroVector)
    , bIsInAir(false)
    , bIsJumpingStart(false)
    , bIsJumpingEnd(false)
    , JumpStartTimer(0.f)
    , bJumpStartTimerActive(false)
{

}

void UBiterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* Owner = TryGetPawnOwner();
    if (!Owner) return;
    const ABiterCharacter* Biter = Cast<ABiterCharacter>(Owner);
    if (!Biter) return;

    Speed = Owner->GetVelocity().Size();
    bIsFlying = Biter->GetCharacterMovement()->IsFlying();
    bIsSwimming = Biter->GetCharacterMovement()->IsSwimming();

    bIsInWater = Biter->IsInWater();
    bIsSwimming = Biter->GetCharacterMovement()->IsSwimming();
    InputData = Biter->InputData;
    const bool bCurrentlyInAir = Biter->GetCharacterMovement()->IsFalling();

    if (!bIsInAir && bCurrentlyInAir)
    {
        bIsJumpingStart = true;
        bIsJumpingEnd = false;
        bJumpStartTimerActive = true;
        JumpStartTimer = 0.f;
    }

    if (bJumpStartTimerActive)
    {
        JumpStartTimer += DeltaSeconds;
        if (JumpStartTimer >= 0.3f)
        {
            bIsJumpingStart = false;
            bIsJumpingEnd = bCurrentlyInAir;
            bJumpStartTimerActive = false;
        }
    }

    if (!bCurrentlyInAir)
    {
        bIsJumpingStart = false;
        bIsJumpingEnd = false;
        bJumpStartTimerActive = false;
        JumpStartTimer = 0.f;
    }

    bIsInAir = bCurrentlyInAir;
}

void UBiterAnimInstance::NotifyJumpPressed()
{
    bIsJumpingStart = true;
    bIsJumpingEnd = false;
    bJumpStartTimerActive = true;
    JumpStartTimer = 0.f;
}
