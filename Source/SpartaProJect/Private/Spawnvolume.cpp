#include "SpawnVolume.h"
#include "Components/BoxComponent.h"


ASpawnvolume::ASpawnvolume()
{
 
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}
AActor* ASpawnvolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* ASpawnvolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue < AccumulateChance)
		{
			return Row;
		}
	}
	
	return nullptr;
	// 0.0 ~ 1.0
	// 랜덤값 0.7 = Item B 0.8이 출력
	// 
	// Item A 0.5
	// Item B 0.8
	// Item c 1.0

	// 복권 상자안에 색갈이 다른 공이 
	// 빨간 5공 , 파란 3개, 초록 2개 / 50%, 30%, 20% 전체 10개
	// 0 ~ 1까지 랜덤 숫자를 뽑을 떄 만약에 0.3이 나오면 빨간공
	// 누적확률로 구한다.
}

FVector ASpawnvolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	//(200, 100, 50) (2, 1, 1) = 400 100 50
	// 중심에서 끝까지의 거리 
	FVector BoxOrigin = SpawningBox->GetComponentLocation();//중심 좌표

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	); // 3) 각 축별로 -Extent ~ +Extent 범위의 무작위 값 생성
}

AActor* ASpawnvolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
        ItemClass,
		GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );

	return SpawnedActor;
}

