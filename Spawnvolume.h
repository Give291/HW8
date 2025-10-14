#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "Spawnvolume.generated.h"

class UBoxComponent;


UCLASS()
class SPARTAPROJECT_API ASpawnvolume : public AActor
{
	GENERATED_BODY()

public:
	ASpawnvolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;


	
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();

	FItemSpawnRow* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPointInVolume() const;

	/*UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPointInVolume() const;*/

	//UFUNCTION(BlueprintCallable, Category = "Spawning")
	//void SpawnItem(TSubclassOf<AActor> ItemClass);
	// 
	// TSubclassOf : 탬플릿 이렇게 지정할 경우 액터의 하위 클래스가 아닐 경우 오류가 난다. 액터의 하위클래스까지 포함.



};