// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"
#include "MechGameCharacter.h"
#include "MechPlayerController.h"


ATestGameMode::ATestGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/EngineerMech")); 
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
		
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();	

}

void ATestGameMode::UpdatePlayerList()
{
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
		return myPlayerController->GetDefaultPawn();
	}
	return DefaultPawnClass;
}
