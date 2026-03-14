#include "WeatherManagerActor.h"
#include "EngineUtils.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/DirectionalLight.h" 
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/PostProcessComponent.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Engine/Engine.h"

AWeatherManagerActor::AWeatherManagerActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeatherManagerActor::BeginPlay()
{
    Super::BeginPlay();

    for (TActorIterator<AUltraDynamicSkyActorBase> It(GetWorld()); It; ++It)
    {
        DynamicSky = *It;
        break;
    }
}

void AWeatherManagerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ApplyCurrentWeather();

    if (!IsValid(DynamicSky))
    {
        for (TActorIterator<AUltraDynamicSkyActorBase> It(GetWorld()); It; ++It)
        {
            DynamicSky = *It;
            break;
        }
        if (!IsValid(DynamicSky)) return;
    }   

    WeatherTimer += DeltaTime;
    if (WeatherTimer >= WeatherDuration)
    {
        WeatherTimer = 0.0f;
        CycleWeather();
    }
}

void AWeatherManagerActor::ApplyCurrentWeather()
{
    if (!DynamicSky) return;

    switch (CurrentWeather)
    {
    case EWeatherType::Sunny:
        ApplySunnyWeather();
        break;
    case EWeatherType::Cloudy:
        ApplyCloudyWeather();
        break;
    case EWeatherType::Rainy:
        ApplyRainyWeather();
        break;
    case EWeatherType::Stormy:
        ApplyStormyWeather();
        break;
    }
}

void AWeatherManagerActor::CycleWeather()
{
    switch (CurrentWeather)
    {
    case EWeatherType::Sunny:
        CurrentWeather = EWeatherType::Cloudy;
        break;

    case EWeatherType::Cloudy:
        CurrentWeather = EWeatherType::Rainy;
        break;

    case EWeatherType::Rainy:
        CurrentWeather = EWeatherType::Stormy;
        break;
    case EWeatherType::Stormy:
        CurrentWeather = EWeatherType::Sunny;
        break;
    default:
        CurrentWeather = EWeatherType::Sunny;
        break;
    }
}

void AWeatherManagerActor::ApplySunnyWeather()
{
    if (!IsValid(DynamicSky))
    {
        return;
    }

    if (IsValid(DynamicSky))
    {
        DynamicSky->SetRainActive(false);
        DynamicSky->SetLightningActive(false);
    }

    if (IsValid(DynamicSky->ExponentialFogComponent))
    {
        DynamicSky->ExponentialFogComponent->SetFogDensity(0.0005f);
        DynamicSky->ExponentialFogComponent->SetFogHeightFalloff(0.1f);
    }

    if (IsValid(DynamicSky->CloudMaterialInstance))
    {
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("Cloud_GlobalDensity"), 0.002f);
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("StormClouds"), 0.002f);
    }

    if (IsValid(DynamicSky->SkyLightComponent))
    {
        DynamicSky->bOverrideSkyLight = true;
        DynamicSky->OverriddenSkyLightIntensity = 1.2f;
        DynamicSky->UpdateSkyLight();
    }

    if (IsValid(DynamicSky) && DynamicSky->PostProcessComponent)
    {
        DynamicSky->CurrentColorSaturation = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
        DynamicSky->UpdatePostProcess();
    }

    if (IsValid(DynamicSky->SunLightActor))
    {
        DynamicSky->DayColor = FLinearColor(1.0f, 0.956f, 0.84f);
        DynamicSky->UpdateSun();
    }

    if (IsValid(DynamicSky->MoonLightActor))
    {
        DynamicSky->MoonColor = DynamicSky->MoonColorClear;
        DynamicSky->UpdateMoon();
    }
    
}

void AWeatherManagerActor::ApplyCloudyWeather()
{
    if (!IsValid(DynamicSky))
    {
        return;
    }

    if (IsValid(DynamicSky))
    {
        DynamicSky->SetRainActive(false);
        DynamicSky->SetLightningActive(false);
    }

    if (IsValid(DynamicSky->ExponentialFogComponent))
    {
        DynamicSky->ExponentialFogComponent->SetFogDensity(0.008f);
        DynamicSky->ExponentialFogComponent->SetFogHeightFalloff(0.3f);
    }

    if (IsValid(DynamicSky->CloudMaterialInstance))
    {
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("Cloud_GlobalDensity"), 1.0f);
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("StormClouds"), 1.0f);
    }

    if (IsValid(DynamicSky->SkyLightComponent))
    {
        DynamicSky->bOverrideSkyLight = true;
        DynamicSky->OverriddenSkyLightIntensity = 0.2f;
        DynamicSky->UpdateSkyLight();
    }

    if (IsValid(DynamicSky) && DynamicSky->PostProcessComponent)
    {
        DynamicSky->CurrentColorSaturation = FVector4(0.6f, 0.6f, 0.6f, 1.0f);
        DynamicSky->UpdatePostProcess();
    }

    if (IsValid(DynamicSky->SunLightActor))
    {
        if (ULightComponent* LightComp = DynamicSky->SunLightActor->GetLightComponent())
        {
            DynamicSky->DayColor = DynamicSky->CloudyDayColor;
            DynamicSky->UpdateSun();
        }
    }

    if (IsValid(DynamicSky->MoonLightActor))
    {
        if (ULightComponent* MoonComp = DynamicSky->MoonLightActor->GetLightComponent())
        {
            DynamicSky->MoonColor = DynamicSky->MoonColorCloudy;
            DynamicSky->UpdateMoon();
        }
    }
}

void AWeatherManagerActor::ApplyRainyWeather()
{
    if (!IsValid(DynamicSky)) return;

    if (IsValid(DynamicSky))
    {
        DynamicSky->SetRainActive(true);
        DynamicSky->SetLightningActive(true);
    }

    if (IsValid(DynamicSky->RainNiagaraComponent))
    {
        DynamicSky->RainNiagaraComponent->SetVariableFloat(TEXT("User.SpawnRate"), 2500.0f);
    }

    if (IsValid(DynamicSky->ExponentialFogComponent))
    {
        DynamicSky->ExponentialFogComponent->SetFogDensity(0.015f);
        DynamicSky->ExponentialFogComponent->SetFogHeightFalloff(0.1f);
    }

    if (IsValid(DynamicSky->CloudMaterialInstance))
    {
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("Cloud_GlobalDensity"), 0.5f);
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("StormClouds"), 0.6f);
    }

    if (IsValid(DynamicSky->SkyLightComponent))
    {
        DynamicSky->bOverrideSkyLight = true;
        DynamicSky->OverriddenSkyLightIntensity = 0.1f;
        DynamicSky->UpdateSkyLight();
    }

    if (IsValid(DynamicSky->PostProcessComponent))
    {
        DynamicSky->CurrentColorSaturation = FVector4(0.4f, 0.4f, 0.4f, 1.0f);
        DynamicSky->UpdatePostProcess();
    }

    if (IsValid(DynamicSky->SunLightActor))
    {
        DynamicSky->DayColor = DynamicSky->RainyDayColor;
        DynamicSky->UpdateSun();
    }

    if (IsValid(DynamicSky->MoonLightActor))
    {
        DynamicSky->MoonColor = DynamicSky->MoonColorRainy;
        DynamicSky->UpdateMoon();
    }
    
    
}

void AWeatherManagerActor::ApplyStormyWeather()
{
    if (!IsValid(DynamicSky)) return;

    DynamicSky->SetRainActive(true);

    if (IsValid(DynamicSky->RainNiagaraComponent))
    {
        DynamicSky->RainNiagaraComponent->SetVariableFloat(TEXT("User.SpawnRate"), 5000.0f);
    }

    if (IsValid(DynamicSky->ExponentialFogComponent))
    {
        DynamicSky->ExponentialFogComponent->SetFogDensity(0.03f);
        DynamicSky->ExponentialFogComponent->SetFogHeightFalloff(0.08f);
    }

    if (IsValid(DynamicSky->CloudMaterialInstance))
    {
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("Cloud_GlobalDensity"), 0.9f);
        DynamicSky->CloudMaterialInstance->SetScalarParameterValue(FName("StormClouds"), 1.0f);
    }

    if (IsValid(DynamicSky->SkyLightComponent))
    {
        DynamicSky->bOverrideSkyLight = true;
        DynamicSky->OverriddenSkyLightIntensity = 0.05f;
        DynamicSky->UpdateSkyLight();
    }

    if (IsValid(DynamicSky->PostProcessComponent))
    {
        DynamicSky->CurrentColorSaturation = FVector4(0.2f, 0.2f, 0.2f, 1.0f);
        DynamicSky->UpdatePostProcess();
    }

    if (IsValid(DynamicSky->SunLightActor))
    {
        DynamicSky->DayColor = DynamicSky->RainyDayColor;
        DynamicSky->UpdateSun();
    }

    if (IsValid(DynamicSky->MoonLightActor))
    {
        DynamicSky->MoonColor = DynamicSky->MoonColorRainy;
        DynamicSky->UpdateMoon();
    }    
}