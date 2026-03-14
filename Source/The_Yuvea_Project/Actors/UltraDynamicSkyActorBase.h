#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DirectionalLight.h>
#include <Components/ExponentialHeightFogComponent.h>
#include <Components/VolumetricCloudComponent.h>
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "UltraDynamicSkyActorBase.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API AUltraDynamicSkyActorBase : public AActor
{
	GENERATED_BODY()
	
public:
    AUltraDynamicSkyActorBase();

protected:
    virtual void BeginPlay() override;
    void InitializeSkySettings();
    void SetupSkyAndCloudVisuals();
    void SetupWeatherEffects();
    virtual void Tick(float DeltaTime) override;
    void ConfigureRainNiagara();

public:    
    void UpdateLighting();

    UFUNCTION(BlueprintCallable, Category = "Weather")
    void SetRainActive(bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Weather")
    void SetLightningActive(bool bActive);

    void UpdateSun();
    void UpdateMoon();
    void UpdateSkyLight();
    void UpdateFog();
    void UpdateSkyAtmosphere();
    void UpdatePostProcess();
    void UpdateStarsVisibilityAndIntensity();
    void UpdateClouds();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float TimeOfDay = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    bool bCycleActive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float DayDurationInSeconds = 24.0f*12.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    ADirectionalLight* MoonLightActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    ADirectionalLight* SunLightActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    USkyAtmosphereComponent* SkyAtmosphereComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    USkyLightComponent* SkyLightComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
    UStaticMeshComponent* SkyMesh;

    UPROPERTY(VisibleAnywhere, Category = "Time")
    UStaticMeshComponent* SunVisual;

    UPROPERTY(VisibleAnywhere, Category = "Time")
    UStaticMeshComponent* MoonVisual;

    UPROPERTY(EditAnywhere)
    class UPostProcessComponent* PostProcessComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    UExponentialHeightFogComponent* ExponentialFogComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    UVolumetricCloudComponent* VolumetricCloudComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float MaxMoonIntensity = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float MaxSunIntensity = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FLinearColor MoonColor = FLinearColor(0.35f, 0.4f, 0.55f);

    float TimeSpeed;    

    UFUNCTION(BlueprintCallable)
    float GetTimeOfDay() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float MaxStarIntensity = 5.0f;

    UPROPERTY(Transient)
    UMaterialInstanceDynamic* StarMaterialInstance;

    UPROPERTY()
    UMaterialInterface* SkyMaterial;

    UPROPERTY()
    UMaterialInterface* CloudMaterial;

    UPROPERTY(Transient)
    UMaterialInstanceDynamic* CloudMaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    float CurrentFogDensity = 0.0005f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
    bool bOverrideSkyLight = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky")
    float OverriddenSkyLightIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess")
    FVector4 CurrentColorSaturation = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    FLinearColor DayColor = FLinearColor(1.0f, 0.956f, 0.84f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    FLinearColor CloudyDayColor = FLinearColor(0.65f, 0.7f, 0.75f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon")
    FLinearColor MoonColorClear = FLinearColor(0.6f, 0.65f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon")
    FLinearColor MoonColorCloudy = FLinearColor(0.3f, 0.35f, 0.4f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    FLinearColor RainyDayColor = FLinearColor(0.5f, 0.55f, 0.6f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon")
    FLinearColor MoonColorRainy = FLinearColor(0.4f, 0.45f, 0.5f);

    UPROPERTY(EditAnywhere, Category = "Weather")
    UNiagaraSystem* RainNiagaraSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weather")
    UNiagaraComponent* RainNiagaraComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning")
    float OverriddenSunLightIntensity = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Lightning")
    float LightningInterval;

    UPROPERTY(EditAnywhere, Category = "Weather Effects")
    UNiagaraSystem* LightningBoltNiagaraSystem;

private:
    UPROPERTY(VisibleAnywhere, Category = "Lightning")
    bool bLightningSystemEnabled = false;

    FTimerHandle LightningTimerHandle;
    FTimerHandle LightningFlashTimerHandle;
    void StartLightningTimer();
    void StopLightningTimer();
    void TryGenerateLightning();
    void TriggerLightningFlash();
    void EndLightningFlash();

    UPROPERTY(EditAnywhere, Category = "Lightning")
    float MinLightningDelay = 5.0f;
    UPROPERTY(EditAnywhere, Category = "Lightning")
    float MaxLightningDelay = 15.0f; 

    UPROPERTY(EditAnywhere, Category = "Lightning")
    float LightningFlashDuration = 0.25f;

    UPROPERTY(EditAnywhere, Category = "Lightning")
    float MaxThunderDistance = 5000.0f;
};