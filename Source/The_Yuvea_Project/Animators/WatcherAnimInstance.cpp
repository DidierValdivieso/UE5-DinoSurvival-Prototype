#include "WatcherAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Characters/WatcherCharacter.h"

UWatcherAnimInstance::UWatcherAnimInstance()
    : Speed(0.f)
    , InputData(FVector2D::ZeroVector)
    , bIsInAir(false)
    , bIsJumpingStart(false)
    , bIsJumpingEnd(false)
    , JumpStartTimer(0.f)
    , bJumpStartTimerActive(false)
{

}

void UWatcherAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* Owner = TryGetPawnOwner();
    if (!Owner) return;
    const AWatcherCharacter* Watcher = Cast<AWatcherCharacter>(Owner);
    if (!Watcher) return;

    Speed = Owner->GetVelocity().Size();
    bIsFlying = Watcher->GetCharacterMovement()->IsFlying();
    bIsSwimming = Watcher->GetCharacterMovement()->IsSwimming();    

    bIsInWater = Watcher->IsInWater();
    bIsSwimming = Watcher->GetCharacterMovement()->IsSwimming();
    InputData = Watcher->InputData;
    const bool bCurrentlyInAir = Watcher->GetCharacterMovement()->IsFalling();

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

void UWatcherAnimInstance::NotifyJumpPressed()
{
    bIsJumpingStart = true;
    bIsJumpingEnd = false;
    bJumpStartTimerActive = true;
    JumpStartTimer = 0.f;
}

