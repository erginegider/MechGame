// Fill out your copyright notice in the Description page of Project Settings.


#include "MechPlayerState.h"
#include "MechGameCharacter.h"
#include "Net/UnrealNetwork.h"
#include "TestGameMode.h"

AMechPlayerState::AMechPlayerState()
{
	isCharacterSet = false;	
	
	
}



void AMechPlayerState::CopyProperties(APlayerState * PlayerState)
{
	Super::CopyProperties(PlayerState);
	
	AMechPlayerState *NewState = Cast<AMechPlayerState>(PlayerState);

	if (NewState)
	{
		NewState->SelectedPlayer = this->SelectedPlayer;
		NewState->isCharacterSet = this->isCharacterSet;
		
	}
	
}


void AMechPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMechPlayerState, SelectedPlayer);
	DOREPLIFETIME(AMechPlayerState, isCharacterSet);

}