// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalProject2GameMode.h"
#include "FinalProject2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "BasePlayerCharacter.h"

AFinalProject2GameMode::AFinalProject2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/Blueprints/PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
