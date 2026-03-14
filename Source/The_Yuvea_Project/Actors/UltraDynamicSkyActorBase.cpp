#include "UltraDynamicSkyActorBase.h"
#include "Engine/DirectionalLight.h"
#include "GameFramework/Actor.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/SkyLight.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "The_Yuvea_Project/Characters/MainCharacter.h"

AUltraDynamicSkyActorBase::AUltraDynamicSkyActorBase()
{
    PrimaryActorTick.bCanEverTick = true;

    SkyAtmosphereComponent = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
    SkyAtmosphereComponent->SetupAttachment(RootComponent);

    SkyLightComponent = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
    SkyLightComponent->SetupAttachment(RootComponent);
    SkyLightComponent->Mobility = EComponentMobility::Movable;
    SkyLightComponent->SetRealTimeCapture(true);

    ExponentialFogComponent = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialFog"));
    ExponentialFogComponent->SetupAttachment(RootComponent);
    ExponentialFogComponent->SetVolumetricFog(true);

    VolumetricCloudComponent = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloud"));
    VolumetricCloudComponent->SetupAttachment(RootComponent);

    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
    PostProcessComponent->SetupAttachment(RootComponent);

    PostProcessComponent->Settings.bOverride_AutoExposureMethod = true;
    PostProcessComponent->Settings.AutoExposureMethod = EAutoExposureMethod::AEM_Histogram;
    PostProcessComponent->Settings.bOverride_AutoExposureMinBrightness = true;
    PostProcessComponent->Settings.bOverride_AutoExposureMaxBrightness = true;
    PostProcessComponent->Settings.AutoExposureMinBrightness = 0.3f;
    PostProcessComponent->Settings.AutoExposureMaxBrightness = 2.0f;

    SkyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkyMesh"));
    SkyMesh->SetupAttachment(RootComponent);
    SkyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SkyMesh->SetCastShadow(false);
    SkyMesh->SetRelativeScale3D(FVector(1000.0f));

    SunVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SunVisual"));
    SunVisual->SetupAttachment(RootComponent);
    SunVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SunVisual->SetCastShadow(false);
    SunVisual->SetRelativeScale3D(FVector(80.f));
    SunVisual->SetVisibility(true);

    MoonVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoonVisual"));
    MoonVisual->SetupAttachment(RootComponent);
    MoonVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MoonVisual->SetCastShadow(false);
    MoonVisual->SetRelativeScale3D(FVector(65.f));
    MoonVisual->SetVisibility(true);

    ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMesh.Succeeded())
    {
        SunVisual->SetStaticMesh(SphereMesh.Object);
        MoonVisual->SetStaticMesh(SphereMesh.Object);
    }

    ConstructorHelpers::FObjectFinder<UMaterialInterface> SunMat(TEXT("/Game/Materials/M_Sun"));
    ConstructorHelpers::FObjectFinder<UMaterialInterface> MoonMat(TEXT("/Game/Materials/M_Moon"));
    if (SunMat.Succeeded())
    {
        SunVisual->SetMaterial(0, SunMat.Object);
    }
    if (MoonMat.Succeeded())
    {
        MoonVisual->SetMaterial(0, MoonMat.Object);
    }

    ConstructorHelpers::FObjectFinder<UMaterialInterface> SkyMatObj(TEXT("/Game/Materials/M_Procedural_Sky_Night"));
    if (SkyMatObj.Succeeded())
    {
        SkyMaterial = SkyMatObj.Object;
    }

    ConstructorHelpers::FObjectFinder<UMaterialInterface> CloudMatObj(TEXT("/Game/Materials/m_SimpleVolumetricCloud_Inst"));
    if (CloudMatObj.Succeeded())
    {
        CloudMaterial = CloudMatObj.Object;
    }
}

void AUltraDynamicSkyActorBase::BeginPlay()
{
    Super::BeginPlay();
    InitializeSkySettings();
    SetupSkyAndCloudVisuals();
    SetupWeatherEffects();
}



void AUltraDynamicSkyActorBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsValid(SkyMesh))
    {
        return;
    }

    if (!IsValid(StarMaterialInstance))
    {
        return;
    }

    if (!IsValid(VolumetricCloudComponent))
    {
        return;
    }

    TimeOfDay += TimeSpeed * DeltaTime;
    if (TimeOfDay >= 24.0f)
        TimeOfDay -= 24.0f;

    if (bCycleActive)
    {
        UpdateLighting();
    }
}

void AUltraDynamicSkyActorBase::UpdateLighting()
{
    UpdateSun();
    UpdateMoon();
    UpdateSkyLight();
    UpdateFog();
    UpdateSkyAtmosphere();
    UpdatePostProcess();
    UpdateStarsVisibilityAndIntensity();
    UpdateClouds();
}

void AUltraDynamicSkyActorBase::ConfigureRainNiagara()
{
    if (!RainNiagaraComponent) return;

    RainNiagaraComponent->SetVisibility(false);
    RainNiagaraComponent->Activate(true);
}

void AUltraDynamicSkyActorBase::SetRainActive(bool bActive)
{
    if (RainNiagaraComponent)
    {
        RainNiagaraComponent->SetVisibility(bActive, true);
    }
}

void AUltraDynamicSkyActorBase::InitializeSkySettings()
{
    TimeOfDay = 6.0f;

    if (DayDurationInSeconds <= 0)
    {
        DayDurationInSeconds = 1200.0f;
    }
    TimeSpeed = 24.0f / DayDurationInSeconds;

    if (IsValid(PostProcessComponent))
    {
        PostProcessComponent->bUnbound = true;
    }
}

void AUltraDynamicSkyActorBase::SetupSkyAndCloudVisuals()
{
    if (!IsValid(SkyMesh) || !IsValid(SkyMaterial))
    {
    }
    else
    {
        StarMaterialInstance = UMaterialInstanceDynamic::Create(SkyMaterial, this, FName("StarMID"));
        if (IsValid(StarMaterialInstance))
        {
            SkyMesh->SetMaterial(0, StarMaterialInstance);
        }
    }

    if (IsValid(VolumetricCloudComponent) && IsValid(CloudMaterial))
    {
        CloudMaterialInstance = UMaterialInstanceDynamic::Create(CloudMaterial, this, FName("CloudMID"));
        if (IsValid(CloudMaterialInstance))
        {
            VolumetricCloudComponent->SetMaterial(CloudMaterialInstance);
        }
    }
}

void AUltraDynamicSkyActorBase::SetupWeatherEffects()
{
    APawn* PlayerPawn = GetWorld() ? GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr : nullptr;
    AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(PlayerPawn);

    USceneComponent* AttachTargetComponent = nullptr;
    FVector RelativeLocation = FVector(0, 0, 2000);

    if (IsValid(PlayerCharacter))
    {
        if (IsValid(PlayerCharacter->GetCapsuleComponent()))
        {
            AttachTargetComponent = PlayerCharacter->GetCapsuleComponent();
            RelativeLocation = FVector(0, 0, PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 100.0f);
            RelativeLocation.Z += 200;
        }
    }

    if (IsValid(RainNiagaraSystem))
    {
        if (IsValid(AttachTargetComponent))
        {
            RainNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
                RainNiagaraSystem,
                AttachTargetComponent,
                NAME_None,
                RelativeLocation,
                FRotator::ZeroRotator,
                EAttachLocation::KeepRelativeOffset,
                true
            );
        }
        else
        {
            RainNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                RainNiagaraSystem,
                GetActorLocation() + RelativeLocation,
                FRotator::ZeroRotator,
                FVector(1.0f),
                true,
                true
            );
        }


        if (IsValid(RainNiagaraComponent))
        {
            RainNiagaraComponent->SetVisibility(false);
        }
    }
}

void AUltraDynamicSkyActorBase::SetLightningActive(bool bActive)
{
    bLightningSystemEnabled = bActive;

    if (bActive)
    {
        StartLightningTimer();
    }
    else
    {
        StopLightningTimer();
    }
}

void AUltraDynamicSkyActorBase::StartLightningTimer()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (LightningTimerHandle.IsValid())
    {
        World->GetTimerManager().ClearTimer(LightningTimerHandle);
        LightningTimerHandle.Invalidate();
    }

    float ClampedInterval = FMath::Clamp(LightningInterval, 1.0f, 10.0f);

    World->GetTimerManager().SetTimer(
        LightningTimerHandle,
        this,
        &AUltraDynamicSkyActorBase::TryGenerateLightning,
        ClampedInterval,
        true
    );
}

void AUltraDynamicSkyActorBase::StopLightningTimer()
{
    UWorld* World = GetWorld();
    if (World && LightningTimerHandle.IsValid())
    {
        World->GetTimerManager().ClearTimer(LightningTimerHandle);
        LightningTimerHandle.Invalidate();
    }
}

void AUltraDynamicSkyActorBase::TryGenerateLightning()
{
    if (!bLightningSystemEnabled)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    TriggerLightningFlash();
}

void AUltraDynamicSkyActorBase::TriggerLightningFlash()
{
    UWorld* World = GetWorld();
    if (!World) return;

    FVector Origin = GetActorLocation();
    float Radius = 100000.0f;

    FVector2D RandomOffset2D = FMath::RandPointInCircle(Radius);
    FVector Start = Origin + FVector(RandomOffset2D.X, RandomOffset2D.Y, 1500.0f);
    FVector End = Start - FVector(0, 0, 3000.0f);

    if (LightningBoltNiagaraSystem)
    {
        FVector MidPoint = (Start + End) * 0.5f;
        FRotator Rotation = (End - Start).Rotation();

        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            World,
            LightningBoltNiagaraSystem,
            MidPoint,
            Rotation,
            FVector(1.0f),
            true,
            true,
            ENCPoolMethod::None,
            true
        );
    }

    if (LightningFlashTimerHandle.IsValid())
    {
        World->GetTimerManager().ClearTimer(LightningFlashTimerHandle);
    }

    World->GetTimerManager().SetTimer(
        LightningFlashTimerHandle,
        this,
        &AUltraDynamicSkyActorBase::EndLightningFlash,
        LightningFlashDuration,
        false
    );
}


void AUltraDynamicSkyActorBase::EndLightningFlash()
{
    if (IsValid(SkyLightComponent))
    {
        SkyLightComponent->SetIntensity(OverriddenSkyLightIntensity);
    }

    if (IsValid(SunLightActor))
    {
        UDirectionalLightComponent* SunLightComponent = Cast<UDirectionalLightComponent>(SunLightActor->GetLightComponent());
        if (SunLightComponent)
        {
            SunLightComponent->SetIntensity(OverriddenSunLightIntensity);
        }
    }

    LightningFlashTimerHandle.Invalidate();
}

void AUltraDynamicSkyActorBase::UpdateSun()
{
    float SunAngle = ((TimeOfDay - 12.0f) / 24.0f) * 360.0f;
    FVector BaseLocation = GetActorLocation();

    if (SunLightActor)
    {
        FQuat SunQuat = FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(SunAngle));
        FVector SunDirection = SunQuat.RotateVector(FVector(0, 0, 1));
        FVector SunLocation = BaseLocation + SunDirection * 100000.0f;
        FVector LookAtDirection = (BaseLocation - SunLocation).GetSafeNormal();
        FRotator SunRot = LookAtDirection.Rotation();
        SunLightActor->SetActorRotation(SunRot);

        if (UDirectionalLightComponent* SunDirLight = Cast<UDirectionalLightComponent>(SunLightActor->GetLightComponent()))
        {
            SunDirLight->ForwardShadingPriority = 1;
            SunDirLight->SetAtmosphereSunLightIndex(0);
        }

        if (ULightComponent* LightComp = SunLightActor->GetLightComponent())
        {
            const bool bSunAboveHorizon = SunDirection.Z > 0.0f;

            float TimeRatio = FMath::Fmod(TimeOfDay, 24.0f) / 24.0f;
            float DayFactor = FMath::Clamp(FMath::Sin(TimeRatio * PI), 0.0f, 1.0f);

            LightComp->SetIntensity(bSunAboveHorizon ? DayFactor * MaxSunIntensity : 1.0f);
            LightComp->SetLightColor(DayColor);
        }

        if (SunVisual)
        {
            SunVisual->SetWorldLocation(BaseLocation + SunDirection * 100000.0f);
            SunVisual->SetVisibility(SunDirection.Z > 0.0f);
        }
    }
}

void AUltraDynamicSkyActorBase::UpdateMoon()
{
    float SunAngle = ((TimeOfDay - 12.0f) / 24.0f) * 360.0f;
    float MoonAngle = SunAngle + 180.0f;
    FVector BaseLocation = GetActorLocation();

    if (MoonLightActor)
    {
        FQuat MoonQuat = FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(MoonAngle));
        FVector MoonDirection = MoonQuat.RotateVector(FVector(0, 0, 1));
        FVector MoonLocation = BaseLocation + MoonDirection * 100000.0f;
        FVector LookAtDirection = (BaseLocation - MoonLocation).GetSafeNormal();
        FRotator MoonRot = LookAtDirection.Rotation();
        MoonLightActor->SetActorRotation(MoonRot);

        if (UDirectionalLightComponent* MoonDirLight = Cast<UDirectionalLightComponent>(MoonLightActor->GetLightComponent()))
        {
            MoonDirLight->ForwardShadingPriority = 0;
            MoonDirLight->SetAtmosphereSunLightIndex(1);
        }

        if (ULightComponent* MoonComp = MoonLightActor->GetLightComponent())
        {
            const bool bMoonAboveHorizon = MoonDirection.Z > 0.0f;

            float TimeRatio = FMath::Fmod(TimeOfDay, 24.0f) / 24.0f;
            float DayFactor = FMath::Clamp(FMath::Sin(TimeRatio * PI), 0.0f, 1.0f);
            float NightFactor = 1.0f - DayFactor;

            MoonComp->SetIntensity(bMoonAboveHorizon ? NightFactor * MaxMoonIntensity : 1.0f);
            MoonComp->SetLightColor(MoonColor);
        }

        if (MoonVisual)
        {
            MoonVisual->SetWorldLocation(BaseLocation + MoonDirection * 100000.0f);
            MoonVisual->SetVisibility(MoonDirection.Z > 0.0f);
        }
    }
}

void AUltraDynamicSkyActorBase::UpdateSkyLight()
{
    if (SkyLightComponent)
    {
        if (bOverrideSkyLight)
        {
            SkyLightComponent->SetIntensity(OverriddenSkyLightIntensity);
        }
        else
        {
            float TimeRatio = FMath::Fmod(TimeOfDay, 24.0f) / 24.0f;
            float DayFactor = FMath::Clamp(FMath::Sin(TimeRatio * PI), 0.0f, 1.0f);
            SkyLightComponent->SetIntensity(FMath::Lerp(0.1f, 1.0f, DayFactor));
        }

        SkyLightComponent->RecaptureSky();
    }
}

void AUltraDynamicSkyActorBase::UpdateFog()
{
    if (ExponentialFogComponent)
    {
        float TimeRatio = FMath::Fmod(TimeOfDay, 24.0f) / 24.0f;
        float DayFactor = FMath::Clamp(FMath::Sin(TimeRatio * PI), 0.0f, 1.0f);

        FLinearColor NightFogColor(0.03f, 0.04f, 0.08f);
        FLinearColor DayFogColor(0.5f, 0.5f, 0.6f);
        FLinearColor CurrentFogColor = FMath::Lerp(NightFogColor, DayFogColor, DayFactor);

        ExponentialFogComponent->SetFogInscatteringColor(CurrentFogColor);

        ExponentialFogComponent->SetFogDensity(CurrentFogDensity);
        ExponentialFogComponent->SetVisibility(true);
        ExponentialFogComponent->MarkRenderStateDirty();
    }
}

void AUltraDynamicSkyActorBase::UpdateSkyAtmosphere()
{
    if (SkyAtmosphereComponent)
    {
        float TimeRatio = FMath::Fmod(TimeOfDay, 24.0f) / 24.0f;
        float DayFactor = FMath::Clamp(FMath::Sin(TimeRatio * PI), 0.0f, 1.0f);

        float ScatteringScale = FMath::Lerp(0.05f, 1.0f, DayFactor);
        SkyAtmosphereComponent->SetRayleighScatteringScale(ScatteringScale);
        SkyAtmosphereComponent->SetMieScatteringScale(ScatteringScale);
    }
}

void AUltraDynamicSkyActorBase::UpdatePostProcess()
{
    if (PostProcessComponent)
    {
        float TimeRatio = FMath::Fmod(TimeOfDay, 24.0f) / 24.0f;
        float DayFactor = FMath::Clamp(FMath::Sin(TimeRatio * PI), 0.0f, 1.0f);

        float MinExp = FMath::Lerp(0.3f, 1.0f, DayFactor);
        float MaxExp = FMath::Lerp(0.6f, 2.0f, DayFactor);
        PostProcessComponent->Settings.AutoExposureMinBrightness = MinExp;
        PostProcessComponent->Settings.AutoExposureMaxBrightness = MaxExp;

        PostProcessComponent->Settings.ColorSaturation = CurrentColorSaturation;
    }
}

void AUltraDynamicSkyActorBase::UpdateStarsVisibilityAndIntensity()
{
    if (!IsValid(StarMaterialInstance) || !IsValid(SkyMesh)) return;

    if (SkyMesh && StarMaterialInstance)
    {
        const float StarVisibility = (TimeOfDay >= 18.0f || TimeOfDay < 6.0f) ? 0.0f : 1.0f;
        StarMaterialInstance->SetScalarParameterValue(FName("StarVisibility"), StarVisibility);
    }
}

void AUltraDynamicSkyActorBase::UpdateClouds()
{
    if (CloudMaterialInstance)
    {
        CloudMaterialInstance->SetScalarParameterValue(FName("WindSpeed"), 1.0f);
    }
}

float AUltraDynamicSkyActorBase::GetTimeOfDay() const
{
    return TimeOfDay;
}