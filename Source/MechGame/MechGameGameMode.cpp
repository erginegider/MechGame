// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechGameGameMode.h"
#include "MechGameCharacter.h"
#include "MechPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMechGameGameMode::AMechGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/SupportMesh")); 
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	bUseSeamlessTravel = true;
}

UClass * AMechGameGameMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	AMechPlayerController *OurPlayerController = Cast<AMechPlayerController>(InController);
	if (OurPlayerController)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("%s"), *OurPlayerController->DefaultPawn.Get()->GetName()));
		return OurPlayerController->DefaultPawn.Get();
		
	} else
	return Super::GetDefaultPawnClassForController_Implementation(InController);
	
}
