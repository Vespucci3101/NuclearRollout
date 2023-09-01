// Copyright Epic Games, Inc. All Rights Reserved.

#include "OscarKiloTangoGameMode.h"
#include "OscarKiloTangoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOscarKiloTangoGameMode::AOscarKiloTangoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
