#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SparataCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;


UCLASS()
class SPARTAPROJECT_API ASparataCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASparataCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") // 수정이 불가능 하지만, 내부 속성 정도는 조정이 가능하다
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);


	//Scene랑 비슷하지만 여기서는 UPROPERTY를 사용하여 설정해서 Static 지정을 안해주고 사용 

protected:


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//입력 바인딩 함수, Enhanced에 등록이 되어야 애네들이 인지가 가능하다
	UFUNCTION()
	void Move(const FInputActionValue& value); // 구조체의 경우 값이 크기 때문에 &를 사용해서 참조를 가져오는게 좋음
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);


	// 캐릭터의 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	void OnDeath();
	void UpdateOverheadHP();

	virtual void BeginPlay() override;
	
	virtual float TakeDamage(
		float DamageAmount, // 데미지를 얼마나 받았는지
		struct FDamageEvent const& DamageEvent, // 데미지를 받은 어떤 유형
		AController* EventInstigator, // 데미지를 누가 입혔는지
		AActor* DamagerCauser) override; // 데미지를 일으킨 오브젝트

	

private:
	float NormalSpeed;
	float SprintSpeedMultipltier;
	float SprintSpeed;
};
