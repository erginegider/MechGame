// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "MechGameCharacter.h"
#include "MechGameGameMode.h"
#include "Net/UnrealNetwork.h"
#include "MechGameInstance.h"
#include "MechPlayerState.h"



void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyPlayerController::Server_Clicked_Implementation(AMechGameCharacter *selectedpawn)
{
	
	
	AMechPlayerState *MechPlayerState= Cast<AMechPlayerState>(PlayerState);
	
	if (MechPlayerState)
	{
		MechPlayerState->SelectedPlayer = selectedpawn->GetClass();
		MechPlayerState->isCharacterSet = true;
		AMechGameGameMode *GameMode = Cast<AMechGameGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			
			GameMode->ServerTravel();

		}

	}		
	
	

}

bool ALobbyPlayerController::Server_Clicked_Validate(AMechGameCharacter *selectedpawn)
{
	return true;
}



void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALobbyPlayerController, TeamID);

}