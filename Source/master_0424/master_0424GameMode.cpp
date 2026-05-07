// Copyright Epic Games, Inc. All Rights Reserved.

#include "master_0424GameMode.h"
#include "master_0424Character.h"
#include "UObject/ConstructorHelpers.h"

Amaster_0424GameMode::Amaster_0424GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
