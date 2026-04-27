// Copyright Epic Games, Inc. All Rights Reserved.

#include "camera3dPrecticeGameMode.h"
#include "camera3dPrecticeCharacter.h"
#include "UObject/ConstructorHelpers.h"

Acamera3dPrecticeGameMode::Acamera3dPrecticeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
