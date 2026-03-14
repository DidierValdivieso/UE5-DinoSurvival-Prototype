#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RottenEatingZoneVolumeActor.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API ARottenEatingZoneVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARottenEatingZoneVolumeActor();

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* BoxComponent;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
