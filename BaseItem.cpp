#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ABaseItem::ABaseItem()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject <USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	//�̺�Ʈ ���ε�
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

}

void ABaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp, // OverlappedComp : �ڱ� �ڽ� ������ �ݸ��� ���Ǿ�
	AActor* OtherActor, // OtherActor : ���Ǿ� ���۳�Ʈ�� �ε��� ���۳�Ʈ
	UPrimitiveComponent* OtherComp, // OtherComp : OtherActor�� �پ��ִ� ������ ���۳�Ʈ
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player")) // Tag : ������ �г���, ���� �ε����� �÷��̾ �´��� Ȯ��
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap")));
		ActivateItem(OtherActor);
	}
}

void ABaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}
void ABaseItem::ActivateItem(AActor* Activate)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle)
	{
	Particle = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		PickupParticle,
		GetActorLocation(),
		GetActorRotation(),
		true
	);
	}
	
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			1.5f,
			false
		);
	}
}
FName ABaseItem::GetItemType() const
{
	return  ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}