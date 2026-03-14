#include "GliderAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Characters/GliderCharacter.h"

UGliderAnimInstance::UGliderAnimInstance()
    : Speed(0.f)
    , InputData(FVector2D::ZeroVector)
    , bIsInAir(false)
    , bIsJumpingStart(false)
    , bIsJumpingEnd(false)
    , JumpStartTimer(0.f)
    , bJumpStartTimerActive(false)
{

}

void UGliderAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* Owner = TryGetPawnOwner();
    if (!Owner) return;
    const AGliderCharacter* Glider = Cast<AGliderCharacter>(Owner);
    if (!Glider) return;

    Speed = Owner->GetVelocity().Size();
    bIsFlying = Glider->GetCharacterMovement()->IsFlying();
    bIsSwimming = Glider->GetCharacterMovement()->IsSwimming();

    bIsInWater = Glider->IsInWater();
    bIsSwimming = Glider->GetCharacterMovement()->IsSwimming();
    InputData = Glider->InputData;
    const bool bCurrentlyInAir = Glider->GetCharacterMovement()->IsFalling();

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

void UGliderAnimInstance::NotifyJumpPressed()
{
    bIsJumpingStart = true;
    bIsJumpingEnd = false;
    bJumpStartTimerActive = true;
    JumpStartTimer = 0.f;
}
