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
	// ������ 0.7 = Item B 0.8�� ���
	// 
	// Item A 0.5
	// Item B 0.8
	// Item c 1.0

	// ���� ���ھȿ� ������ �ٸ� ���� 
	// ���� 5�� , �Ķ� 3��, �ʷ� 2�� / 50%, 30%, 20% ��ü 10��
	// 0 ~ 1���� ���� ���ڸ� ���� �� ���࿡ 0.3�� ������ ������
	// ����Ȯ���� ���Ѵ�.
}

FVector ASpawnvolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	//(200, 100, 50) (2, 1, 1) = 400 100 50
	// �߽ɿ��� �������� �Ÿ� 
	FVector BoxOrigin = SpawningBox->GetComponentLocation();//�߽� ��ǥ

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	); // 3) �� �ະ�� -Extent ~ +Extent ������ ������ �� ����
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

