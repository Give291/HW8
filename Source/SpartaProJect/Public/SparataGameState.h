#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SparataGameState.generated.h"


UCLASS()
class SPARTAPROJECT_API ASparataGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	ASparataGameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;


	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void OnCoinCollected();
	void StartLevel();
	void OnLevelTimeUp();
	void EndLevel();
	void UpdateHUD();
};
