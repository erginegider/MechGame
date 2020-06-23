// Fill out your copyright notice in the Description page of Project Settings.


#include "MechPlayerController.h"
#include "MechGameCharacter.h"
#include "Net/UnrealNetwork.h"
#include "MechGame.h"
#include "TestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "MechAttributeSet.h"
#include "HealthArmorUserWidget.h"


#define prints(x)  GEngine->AddOnScreenDebugMessage(-1,12.0f,FColor::Green,FString::Printf(TEXT("%s"),x))
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
	if (const AMechGameCharacter *OtherPawn = Cast<AMechGameCharacter>(&Other))
	{
		if (const IGenericTeamAgentInterface *TeamAgentInterface = Cast<IGenericTeamAgentInterface>(OtherPawn))
		{
				
		    return FGenericTeamId::GetAttitude(GetGenericTeamId(),OtherPawn->GetGenericTeamId());
		}
	}

	return ETeamAttitude::Hostile;
}

void AMechPlayerController::SetGenericTeamId(const FGenericTeamId & NewTeamID)
{
	
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;		
		
	}

	AMechGameCharacter * myCharacter = Cast<AMechGameCharacter>(GetPawn());
	if (myCharacter)
	{
		//
	}
}


void AMechPlayerController::ClientRestart_Implementation(class APawn* NewPawn)
{
	Super::ClientRestart_Implementation(NewPawn);
	AMechGameCharacter *myPawn = Cast<AMechGameCharacter>(NewPawn);
	
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
				
				
			}
			
		}
		if (PlayerReticleClass)
		{
			if (!PlayerReticle)
			{
				PlayerReticle = CreateWidget<UUserWidget>(this, PlayerReticleClass);
				if (!PlayerReticle)return;
				PlayerReticle->AddToViewport();
			}

		}

		AMechGameCharacter *MechGameCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (MechGameCharacter)
		{
			UMechAttributeSet *AttributeSet = MechGameCharacter->MechAttributeSet;
			FGameplayAttribute HealthAtribute = AttributeSet->GetHealthAttribute();
			FGameplayAttribute ArmorAtribute = AttributeSet->GetArmorAttribute();

			UAbilitySystemComponent *AbilitySystemComponent = MechGameCharacter->GetAbilitySystemComponent();

			if (AbilitySystemComponent)
			{
				UHealthArmorUserWidget *UserWidget = Cast<UHealthArmorUserWidget>(PlayerWidget);
				if (UserWidget)
				{
					FOnAttributeChangeData HealthInitData;

					HealthInitData.NewValue = AttributeSet->GetHealth();
					
					AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAtribute).AddUObject(UserWidget, &UHealthArmorUserWidget::UpdateHealthBar);

					AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAtribute).Broadcast(HealthInitData);



					FOnAttributeChangeData ArmorInitData;

					ArmorInitData.NewValue = AttributeSet->GetArmor();

					AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ArmorAtribute).AddUObject(UserWidget, &UHealthArmorUserWidget::UpdateArmorBar);

					AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ArmorAtribute).Broadcast(ArmorInitData);
				}
				
			}

		}
		
		


	}
	


	
}


TSubclassOf<APawn> AMechPlayerController::GetDefaultPawn()
{
	ATestGameMode *MyGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));	

	FString ControllerName = this->GetName();
	int32 index = FCString::Atoi(*ControllerName.Right(1));	

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
	
		ServerSetupTeam(myCharacter);
	}
	
}


void AMechPlayerController::OnUnPossess()
{
	bAutoManageActiveCameraTarget = false;
	Super::OnUnPossess();

}

void AMechPlayerController::ServerSetupTeam_Implementation(AMechGameCharacter * SetPawn)
{

	AMechGameCharacter * myPawn = SetPawn;
	if (myPawn)
	{

		if (myPawn->GetGenericTeamId() == FGenericTeamId::NoTeam)
		{
			myPawn->SetGenericTeamId(GetGenericTeamId());
			
		}
		else
		{
			SetGenericTeamId(myPawn->GetGenericTeamId());
			
		}
		


		myPawn->GetAbilitySystemComponent()->RefreshAbilityActorInfo();
	}
}


bool AMechPlayerController::ServerSetupTeam_Validate(AMechGameCharacter * SetPawn)
{
	return true;
}

