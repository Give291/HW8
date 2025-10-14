#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"


UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};


class SPARTAPROJECT_API IItemInterface
{
	GENERATED_BODY()

	
public:
	UFUNCTION() //  : 엔진에 리플렉션 함수를 등록, 이게 있어야만 런타임때 이 함수를 찾을 수 있어  
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, // OverlappedComp : 자기 자신 원형의 콜리전 스피어
		AActor* OtherActor, // OtherActor : 스피어 컴퍼넌트에 부딪힌 컴퍼넌트
		UPrimitiveComponent* OtherComp, // OtherComp : OtherActor에 붙어있던 원인인 컴퍼넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	// 왜 액터를 하냐 클래스로 안하고, 확장성을 생각해봤을 때는 AACtor가 좋다.

	UFUNCTION() //  : 엔진에 리플렉션 함수를 등록, 이게 있어야만 런타임때 이 함수를 찾을 수 있어  
	virtual void OnItemEndOverlap(
	    UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	virtual void ActivateItem(AActor* Activate) = 0;
	virtual FName GetItemType() const = 0; // 아이템 타입을 얻는 것, 리턴으로 반환값만 받기 때문에 const 사용
	// FName으로 하는 이유는 Fstring을 많이 사용하지만, 타입형에서는 FName으로 하는것이 좋다.
	// 단순하게 비교할때 사용 FName 사용 
	// 0이 없을 없을 경우 가상 함수, 0이 알기로는 무조건 오버라이딩 한다 라고 알고 있음(순수 가상 함수).


	// 지뢰, 힐링, 코인
	// 힐링, 코인 - 즉시 발동형 - 오버랩
	// 지뢰 - 범위 내에 오버랩 - 발동이 5초뒤 폭발 - 오버랩 - 데미지
};
