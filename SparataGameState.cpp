#include "SparataGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SparataPlayerController.h"
#include "Spawnvolume.h"
#include "CoinItem.h"
#include "SparataGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ASparataGameState::ASparataGameState()
{
	Score = 0;
	SpawnCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 5.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ASparataGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASparataGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASparataGameState::GetScore() const
{
	return Score;
}

void ASparataGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USparataGameInstance* SpartaGameInstance = Cast<USparataGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToscore(Amount);
		}
	}
}

void ASparataGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASparataPlayerController* SparataPlayerController = Cast<ASparataPlayerController>(PlayerController))
		{
			SparataPlayerController->ShowGameHUD();
		}
	}


	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USparataGameInstance* SpartaGameInstance = Cast<USparataGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), CurrentLevelIndex + 1);

	SpawnCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnvolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	if (FoundVolumes.Num() > 0)
	{
		ASpawnvolume* SpawnVolume = Cast<ASpawnvolume>(FoundVolumes[0]);
		if (SpawnVolume)
		{
			for (int32 i = 0; i < ItemToSpawn; i++)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnCoinCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASparataGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void ASparataGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASparataGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnCoinCount)

		if (SpawnCoinCount > 0 && CollectedCoinCount >= SpawnCoinCount)
		{
			EndLevel();
		}
}

void ASparataGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USparataGameInstance* SpartaGameInstance = Cast<USparataGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;

			if (CurrentLevelIndex >= MaxLevels)
			{
				OnGameOver();
				return;
			}

			if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
			{
				UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
			}
			else
			{
				OnGameOver();
			}
		}
	}
}

void ASparataGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASparataPlayerController* SparataPlayerController = Cast<ASparataPlayerController>(PlayerController))
		{
			SparataPlayerController->SetPause(true);
			SparataPlayerController->ShowMainMenu(true);
		}
	}
}

void ASparataGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ASparataPlayerController* SparataPlayerController = Cast<ASparataPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = SparataPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USparataGameInstance* SpartaGameInstance = Cast<USparataGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}