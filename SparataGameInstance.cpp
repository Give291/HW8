#include "SparataGameInstance.h"

USparataGameInstance::USparataGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;

}

void USparataGameInstance::AddToscore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}

