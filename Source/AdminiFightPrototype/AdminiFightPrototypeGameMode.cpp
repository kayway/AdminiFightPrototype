// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AdminiFightPrototypeGameMode.h"
#include "AdminiFightPrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAdminiFightPrototypeGameMode::AAdminiFightPrototypeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
