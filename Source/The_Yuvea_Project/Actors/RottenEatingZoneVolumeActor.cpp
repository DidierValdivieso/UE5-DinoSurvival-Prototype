#include "RottenEatingZoneVolumeActor.h"
#include "Components/BoxComponent.h"
#include "../Characters/SmallDinosaurCharacter.h"
#include "../Characters/BatCharacter.h"

ARottenEatingZoneVolumeActor::ARottenEatingZoneVolumeActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;

    BoxComponent->SetBoxExtent(FVector(200.f, 200.f, 100.f));
    BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
    BoxComponent->SetGenerateOverlapEvents(true);
    BoxComponent->SetHiddenInGame(false);
}

void ARottenEatingZoneVolumeActor::BeginPlay()
{
    Super::BeginPlay();

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARottenEatingZoneVolumeActor::OnOverlapBegin);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ARottenEatingZoneVolumeActor::OnOverlapEnd);
}

void ARottenEatingZoneVolumeActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AMainCharacter* Character = Cast<AMainCharacter>(OtherActor))
    {
        Character->SetIsNearFoodZoneRotten(true);
    }


}

void ARottenEatingZoneVolumeActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AMainCharacter* Character = Cast<AMainCharacter>(OtherActor))
    {
        Character->SetIsNearFoodZoneRotten(false);
    }
}

