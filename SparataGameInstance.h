#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SparataGameInstance.generated.h"


UCLASS()
class SPARTAPROJECT_API USparataGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USparataGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;


	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToscore(int32 Amount);
};

