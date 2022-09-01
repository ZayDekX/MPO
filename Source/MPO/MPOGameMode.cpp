// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPOGameMode.h"
#include "MPOPlayerController.h"
#include "MPOCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMPOGameMode::AMPOGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMPOPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}