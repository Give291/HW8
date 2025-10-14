#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{

	ExplosionRadius = 300.0f;
	ExplosionDelay = 5.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
	bHasExploded = false; //여러번 발동이 될 수 있으므로 불변수를 하나 둔다.

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);


}



void AMineItem::ActivateItem(AActor* Activator)
{

	if (bHasExploded) return;
	Super::ActivateItem(Activator);
	// 게임 월드 -> 타이머 매니저가 타이머 핸들러를 관리
	//타이머 핸들러 : 각자의 시계

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,this, &AMineItem::Explode, ExplosionDelay, false);
	

}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		 Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);

		 bHasExploded = true;
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}


	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,  // 데미지를 받을 액터 
				ExplosionDamage,// 데미지의 양
				nullptr, // 데미지를 유발하는 주체, 지뢰를 심은 플레이어 그런게 없기 때문에 nullptr
				this, // 데미지를 입힌 액터 현재 이 클래스의 객체 이므로 this
				UDamageType::StaticClass()
			);
		}
	}

	DestroyItem();

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
