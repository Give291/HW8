#include "HellingItem.h"
#include "SparataCharacter.h"

AHellingItem::AHellingItem()
{

	HealAmount = 20.0f;
	ItemType = "Healing Potion";
}

void AHellingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASparataCharacter* PlayerCharacter = Cast<ASparataCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}

		Destroy();
	}
}
