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

	//이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

}

void ABaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp, // OverlappedComp : 자기 자신 원형의 콜리전 스피어
	AActor* OtherActor, // OtherActor : 스피어 컴퍼넌트에 부딪힌 컴퍼넌트
	UPrimitiveComponent* OtherComp, // OtherComp : OtherActor에 붙어있던 원인인 컴퍼넌트
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player")) // Tag : 일종의 닉네임, 현재 부딪힌게 플레이어가 맞는지 확인
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