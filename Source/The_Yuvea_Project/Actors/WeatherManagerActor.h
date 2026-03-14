#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UltraDynamicSkyActorBase.h"
#include "EngineUtils.h"
#include "Engine/Engine.h" 
#include "WeatherManagerActor.generated.h"

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	Sunny     UMETA(DisplayName = "Sunny"),
	Rainy     UMETA(DisplayName = "Rainy"),
	Stormy     UMETA(DisplayName = "Stormy"),
	Cloudy    UMETA(DisplayName = "Cloudy"),
};

UCLASS()
class THE_YUVEA_PROJECT_API AWeatherManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeatherManagerActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:		
	void ApplyCurrentWeather();
	void CycleWeather();
	void ApplySunnyWeather();
	void ApplyCloudyWeather();	
	void ApplyRainyWeather();
	void ApplyStormyWeather();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	EWeatherType CurrentWeather = EWeatherType::Sunny;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float WeatherDuration = 60.0f;

	float WeatherTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	AUltraDynamicSkyActorBase* DynamicSky;
};
