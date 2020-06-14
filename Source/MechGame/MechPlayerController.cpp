// Fill out your copyright notice in the Description page of Project Settings.


#include "MechPlayerController.h"
#include "MechGameCharacter.h"
#include "Net/UnrealNetwork.h"
#include "MechGame.h"
#include "TestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"


#define prints(x)  GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,FString::Printf(TEXT("%s"),x))
#define printn(x)  GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,FString::Printf(TEXT("%i"),x))



AMechPlayerController::AMechPlayerController()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/WarriorMech"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawn = PlayerPawnBPClass.Class;
	}
	
}


ETeamAttitude::Type AMechPlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn *OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface *TeamAgentInterface = Cast<IGenericTeamAgentInterface>(OtherPawn))
		{
			//return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
			return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Hostile;
}

void AMechPlayerController::SetGenericTeamId(const FGenericTeamId & NewTeamID)
{
	
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;
		
		// @todo notify perception system that a controller changed team ID
	}

	AMechGameCharacter * myCharacter = Cast<AMechGameCharacter>(GetPawn());
	if (myCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 23.0f, FColor::Blue, FString::Printf(TEXT("Character : %s      TeamID  : %i  "),*myCharacter->GetName(), TeamID.GetId()));
	}
}


void AMechPlayerController::ClientRestart_Implementation(class APawn* NewPawn)
{
	Super::ClientRestart_Implementation(NewPawn);
	AMechGameCharacter *myPawn = Cast<AMechGameCharacter>(NewPawn);
	if (myPawn)
	{
	
		myPawn->GetAbilitySystemComponent()->RefreshAbilityActorInfo();
	}
}



void AMechPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMechPlayerController, DefaultPawn);
	DOREPLIFETIME(AMechPlayerController, TeamID);
	
}

void AMechPlayerController::Server_Clicked_Implementation(AMechGameCharacter *selectedpawn)
{
	DefaultPawn = selectedpawn->GetClass();
	
}

bool AMechPlayerController::Server_Clicked_Validate(AMechGameCharacter *selectedpawn)
{
	return true;
}

void AMechPlayerController::OnRep_DefaultPawn()
{
	
}

void AMechPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController())
	{
		if (PlayerWidgetClass)
		{
			if (!PlayerWidget)
			{
				PlayerWidget = CreateWidget<UUserWidget>(this, PlayerWidgetClass);
				if (!PlayerWidget)return;
				PlayerWidget->AddToViewport();
				
				UE_LOG(LogAbilitySystem,Warning,TEXT("I am Alive Controller... "));
			}
			
		}

	}
	//SetGenericTeamId(FGenericTeamId(3));
}


TSubclassOf<APawn> AMechPlayerController::GetDefaultPawn()
{
	ATestGameMode *MyGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));	
	FString PawnName = this->GetName();
	int32 index = FCString::Atoi(*PawnName.Right(1));	
	SetGenericTeamId(FGenericTeamId(uint8(index)));
	return MyGameMode->SpawnPawnMap[index];

}

void AMechPlayerController::AcknowledgePossession(APawn * P)
{
	Super::AcknowledgePossession(P);
	bAutoManageActiveCameraTarget = true;
	AMechGameCharacter * myCharacter = Cast<AMechGameCharacter>(P);
	if (myCharacter)
	{
		myCharacter->GetAbilitySystemComponent()->InitAbilityActorInfo(myCharacter, myCharacter);
		if (myCharacter->GetGenericTeamId() == FGenericTeamId::NoTeam)
		{
			myCharacter->SetGenericTeamId(GetGenericTeamId());
		}
		else
		{
			SetGenericTeamId(myCharacter->GetGenericTeamId());
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("%s    : %i  "),*myCharacter->GetName(),GetGenericTeamId().GetId() ));
	}
	
}


void AMechPlayerController::OnUnPossess()
{
	bAutoManageActiveCameraTarget = false;
	Super::OnUnPossess();

}
