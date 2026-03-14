#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Characters/MainCharacter.h"
#include "YuveaGameInstance.generated.h"

UCLASS()
class THE_YUVEA_PROJECT_API UYuveaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACharacter> SelectedCharacterClass;
};
