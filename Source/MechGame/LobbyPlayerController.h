// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	


	UPROPERTY(VisibleAnywhere, Replicated, Category = "Team")
	FGenericTeamId TeamID;

public:



	UFUNCTION()
	virtual void BeginPlay();


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Clicked(AMechGameCharacter *selectedpawn);	
	void Server_Clicked_Implementation(AMechGameCharacter *selectedpawn);
	bool Server_Clicked_Validate(AMechGameCharacter *selectedpawn);

};
