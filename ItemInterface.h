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
	UFUNCTION() //  : ������ ���÷��� �Լ��� ���, �̰� �־�߸� ��Ÿ�Ӷ� �� �Լ��� ã�� �� �־�  
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, // OverlappedComp : �ڱ� �ڽ� ������ �ݸ��� ���Ǿ�
		AActor* OtherActor, // OtherActor : ���Ǿ� ���۳�Ʈ�� �ε��� ���۳�Ʈ
		UPrimitiveComponent* OtherComp, // OtherComp : OtherActor�� �پ��ִ� ������ ���۳�Ʈ
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	// �� ���͸� �ϳ� Ŭ������ ���ϰ�, Ȯ�强�� �����غ��� ���� AACtor�� ����.

	UFUNCTION() //  : ������ ���÷��� �Լ��� ���, �̰� �־�߸� ��Ÿ�Ӷ� �� �Լ��� ã�� �� �־�  
	virtual void OnItemEndOverlap(
	    UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	virtual void ActivateItem(AActor* Activate) = 0;
	virtual FName GetItemType() const = 0; // ������ Ÿ���� ��� ��, �������� ��ȯ���� �ޱ� ������ const ���
	// FName���� �ϴ� ������ Fstring�� ���� ���������, Ÿ���������� FName���� �ϴ°��� ����.
	// �ܼ��ϰ� ���Ҷ� ��� FName ��� 
	// 0�� ���� ���� ��� ���� �Լ�, 0�� �˱�δ� ������ �������̵� �Ѵ� ��� �˰� ����(���� ���� �Լ�).


	// ����, ����, ����
	// ����, ���� - ��� �ߵ��� - ������
	// ���� - ���� ���� ������ - �ߵ��� 5�ʵ� ���� - ������ - ������
};
