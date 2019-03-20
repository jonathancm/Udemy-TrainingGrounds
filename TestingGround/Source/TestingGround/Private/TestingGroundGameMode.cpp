// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TestingGroundGameMode.h"
#include "FirstPersonHUD.h"
#include "FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestingGroundGameMode::ATestingGroundGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/Character/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFirstPersonHUD::StaticClass();
}
