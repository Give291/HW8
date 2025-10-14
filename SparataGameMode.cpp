#include "SparataGameMode.h"
#include "SparataCharacter.h"
#include "SparataPlayerController.h"
#include "SparataGameState.h"

ASparataGameMode::ASparataGameMode()
{
	DefaultPawnClass = ASparataCharacter::StaticClass();
	PlayerControllerClass = ASparataPlayerController::StaticClass();
	GameStateClass = ASparataGameState::StaticClass();
}