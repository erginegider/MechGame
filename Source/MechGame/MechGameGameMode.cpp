// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechGameGameMode.h"
#include "MechGameCharacter.h"
#include "MechPlayerController.h"
#include "LobbyPlayerController.h"
#include "MechPlayerState.h"
#include "MechGameState.h"
#include "UObject/ConstructorHelpers.h"


AMechGameGameMode::AMechGameGameMode()
{

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/EngineerMech"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	
	bUseSeamlessTravel = true;

	PlayerStateClass = AMechPlayerState::StaticClass();
	GameStateClass = AMechGameState::StaticClass();
	
}


UClass * AMechGameGameMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	ALobbyPlayerController *OurPlayerController = Cast<ALobbyPlayerController>(InController);
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
	
}


void AMechGameGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyPlayerController *PlayerController = Cast<ALobbyPlayerController>(NewPlayer);
		
}

void AMechGameGameMode::ServerTravel()
{
	AMechGameState * MechGameState = Cast<AMechGameState>(GameState);
	bool checkalltrue = true;

	if (GameState)
	{
		for (auto &PlayerStateItem : GameState->PlayerArray)
		{
			AMechPlayerState *ThisPlayerState = Cast<AMechPlayerState>(PlayerStateItem);

			if (ThisPlayerState)
			{
				if (ThisPlayerState->SelectedPlayer)
				{
					GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT(" This Character Selected..: %s"), *ThisPlayerState->SelectedPlayer->GetName()));
				}
				
				if (ThisPlayerState->isCharacterSet == false)checkalltrue=false;
				
			}
		}
		if (checkalltrue)
		{
			GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
		}
		
	}
}
