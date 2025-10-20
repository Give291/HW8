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
	bHasExploded = false; //������ �ߵ��� �� �� �����Ƿ� �Һ����� �ϳ� �д�.

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);


}



void AMineItem::ActivateItem(AActor* Activator)
{

	if (bHasExploded) return;
	Super::ActivateItem(Activator);
	// ���� ���� -> Ÿ�̸� �Ŵ����� Ÿ�̸� �ڵ鷯�� ����
	//Ÿ�̸� �ڵ鷯 : ������ �ð�

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
				Actor,  // �������� ���� ���� 
				ExplosionDamage,// �������� ��
				nullptr, // �������� �����ϴ� ��ü, ���ڸ� ���� �÷��̾� �׷��� ���� ������ nullptr
				this, // �������� ���� ���� ���� �� Ŭ������ ��ü �̹Ƿ� this
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
