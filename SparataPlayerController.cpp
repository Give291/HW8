#include "SparataPlayerController.h"
#include "SparataGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "SparataGameState.h"
#include "Kismet/GamePlayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

ASparataPlayerController::ASparataPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr),
	  HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainmenuWidgetClass(nullptr),
	  MainmenuWidgetInstance(nullptr)
{
}

void ASparataPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // LocalPlayer 현재 플레이어에 로컬플레이어 객체를 가져와라 화면뷰 같은 거를 관리
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapname = GetWorld()->GetMapName();
	if (CurrentMapname.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ASparataPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASparataPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetClass = nullptr;
	}

	if (MainmenuWidgetInstance)
	{
		MainmenuWidgetInstance->RemoveFromParent();
		MainmenuWidgetClass = nullptr;
	}

	if (MainmenuWidgetClass)
	{
		MainmenuWidgetInstance = CreateWidget<UUserWidget>(this, MainmenuWidgetClass);
		if (MainmenuWidgetInstance)
		{
			MainmenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}

	if (UTextBlock* ButtonText = Cast<UTextBlock>(MainmenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
	{
		if (bIsRestart)
		{
			ButtonText->SetText(FText::FromString(TEXT("Restart")));
		}
		else
		{
			ButtonText->SetText(FText::FromString(TEXT("Start")));
		}
		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainmenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainmenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainmenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USparataGameInstance* SparatGameInstance = Cast<USparataGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), SparatGameInstance->TotalScore)
						));
				}
			}
		}
	}

	if (UTextBlock* ExitButtonText = Cast<UTextBlock>(MainmenuWidgetInstance->GetWidgetFromName(TEXT("ExitButtonText"))))
	{
		ExitButtonText->SetText(FText::FromString(TEXT("Exit")));
	}

	
	if (UButton* ExitButton = Cast<UButton>(MainmenuWidgetInstance->GetWidgetFromName(TEXT("ExitButton"))))
	{
		ExitButton->OnClicked.AddDynamic(this, &ASparataPlayerController::OnExitButtonClicked);
	}
}

void ASparataPlayerController::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
}

void ASparataPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainmenuWidgetInstance)
	{
		MainmenuWidgetInstance->RemoveFromParent();
		MainmenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		ASparataGameState* SparataGameState = GetWorld() ? GetWorld()->GetGameState<ASparataGameState>() : nullptr;
		if (SparataGameState)
		{
			SparataGameState->UpdateHUD();
		}
	}
}

void ASparataPlayerController::StartGame()
{
	if (USparataGameInstance* SparataGameInstance = Cast<USparataGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SparataGameInstance->CurrentLevelIndex = 0;
		SparataGameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}