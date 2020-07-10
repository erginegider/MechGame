// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"
#include "MechGameCharacter.h"
#include "MechPlayerController.h"
#include "LobbyPlayerController.h"
#include "MechPlayerState.h"
#include "MechGameState.h"

ATestGameMode::ATestGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/EngineerMech")); 
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
		
	PlayerStateClass = AMechPlayerState::StaticClass();
	GameStateClass = AMechGameState::StaticClass();
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();	

}


void ATestGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}

UClass * ATestGameMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	
	AMechPlayerController *myPlayerController = Cast<AMechPlayerController>(InController);
	if(myPlayerController)
	{		

		AMechPlayerState *MechPlayerState = Cast<AMechPlayerState>(myPlayerController->PlayerState);
		if (MechPlayerState)
		{
		
				
				if (MechPlayerState->isCharacterSet)
				{
					return MechPlayerState->SelectedPlayer;
				}			
				else
				{
					int32 index = FCString::Atoi(*myPlayerController->GetName().Right(1));
					return SpawnPawnMap[index];

				}
			
		}
		
		
	}
	return DefaultPawnClass;
}
