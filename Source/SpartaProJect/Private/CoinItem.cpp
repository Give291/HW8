#include "CoinItem.h"
#include "Engine/World.h"
#include "SparataGameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASparataGameState* GameState = World->GetGameState<ASparataGameState>())
			{
				GameState->AddScore(PointValue);

				GameState->OnCoinCollected();
			}
		}
		Destroy();
	}
}