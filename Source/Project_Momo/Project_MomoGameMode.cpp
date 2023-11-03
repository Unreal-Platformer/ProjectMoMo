// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_MomoGameMode.h"
#include "Project_MomoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_MomoGameMode::AProject_MomoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
