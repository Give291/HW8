#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HellingItem.generated.h"


UCLASS()
class SPARTAPROJECT_API AHellingItem : public ABaseItem
{
	GENERATED_BODY()
	

public:
	AHellingItem();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;

	virtual void ActivateItem(AActor* Activator) override;

};

