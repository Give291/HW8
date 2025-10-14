#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPROJECT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()

public:
	ABaseItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemIComponent")
	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemIComponent")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemIEffects")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemIEffects")
	USoundBase* PickupSound;



	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, // OverlappedComp : 자기 자신 원형의 콜리전 스피어
		AActor* OtherActor, // OtherActor : 스피어 컴퍼넌트에 부딪힌 컴퍼넌트
		UPrimitiveComponent* OtherComp, // OtherComp : OtherActor에 붙어있던 원인인 컴퍼넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activate) override;
	virtual FName GetItemType() const override;

	virtual void DestroyItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

};
