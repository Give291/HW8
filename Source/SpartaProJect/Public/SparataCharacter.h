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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") // ������ �Ұ��� ������, ���� �Ӽ� ������ ������ �����ϴ�
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);


	//Scene�� ��������� ���⼭�� UPROPERTY�� ����Ͽ� �����ؼ� Static ������ �����ְ� ��� 

protected:


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//�Է� ���ε� �Լ�, Enhanced�� ����� �Ǿ�� �ֳ׵��� ������ �����ϴ�
	UFUNCTION()
	void Move(const FInputActionValue& value); // ����ü�� ��� ���� ũ�� ������ &�� ����ؼ� ������ �������°� ����
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


	// ĳ������ ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	void OnDeath();
	void UpdateOverheadHP();

	virtual void BeginPlay() override;
	
	virtual float TakeDamage(
		float DamageAmount, // �������� �󸶳� �޾Ҵ���
		struct FDamageEvent const& DamageEvent, // �������� ���� � ����
		AController* EventInstigator, // �������� ���� ��������
		AActor* DamagerCauser) override; // �������� ����Ų ������Ʈ

	

private:
	float NormalSpeed;
	float SprintSpeedMultipltier;
	float SprintSpeed;
};
