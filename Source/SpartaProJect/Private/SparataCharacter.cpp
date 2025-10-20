#include "SparataCharacter.h"
#include "SparataPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "SparataGameState.h"


ASparataCharacter::ASparataCharacter()
{
	PrimaryActorTick.bCanEverTick = false; // Tick 함수를 사용 안하므로 false로 설정

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f; // 캐릭터와의 거리
	SpringArmComp->bUsePawnControlRotation = true; // 캐릭터가 움직일 때 스프링암도 같이 움직이게끔


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // 카메라는 고정 스프링 암만 움직이게끔

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OvergeadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen); // 월드로 할지 스크린 할지 정하는건



	NormalSpeed = 600.0f;
	SprintSpeedMultipltier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultipltier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100.0f;
	Health = MaxHealth;

}
// Scene 할때랑 비슷하다고 생각하면 쉬움

void ASparataCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP(); //캐릭터 처음 생성시 hp 초기화
}


void ASparataCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASparataPlayerController* PlayerController = Cast<ASparataPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASparataCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASparataCharacter::StartJump
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASparataCharacter::StopJump
				);

				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASparataCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASparataCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASparataCharacter::StopSprint
				);
			}
		}
	}
}


void ASparataCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return; // 안전코드

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X)) // 입력값이 0이라면 키를 안누른 상태인데 가까운 값이 들어왓을 때 0으로 인정을 해주자
		// 안전코드
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X); 
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}
void ASparataCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}

}
void ASparataCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}
void ASparataCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);

}
void ASparataCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void ASparataCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}


float ASparataCharacter::GetHealth() const
{
	return Health; 
}

void ASparataCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}


float ASparataCharacter::TakeDamage(
	float DamageAmount, 
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamagerCauser
)
{
	float ActualDamage = Super::TakeDamage(
		DamageAmount, 
		DamageEvent, 
		EventInstigator, 
		DamagerCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ASparataCharacter::OnDeath()
{
	//게임 종료 로직을 구현
	ASparataGameState* SparataGamestate = GetWorld() ? GetWorld()->GetGameState<ASparataGameState>() : nullptr;
	if (SparataGamestate)
	{
		SparataGamestate->OnGameOver();
	}
}

void ASparataCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%0.f / %.0f"), Health, MaxHealth)));
	}
}