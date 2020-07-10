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
#include "MechGameGameMode.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#ifndef prints
#define prints(x)  GEngine->AddOnScreenDebugMessage(-1,12.0f,FColor::Green,FString::Printf(TEXT("%s"),x))
#endif

#ifndef printn
#define printn(x)  GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,FString::Printf(TEXT("%i"),x))
#endif



AMechPlayerController::AMechPlayerController()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/WarriorMech"));
	

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));


	

	SightConfig->SightRadius = 10000.0f;
	SightConfig->LoseSightRadius = 15000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMechPlayerController::SenseStuff);
	PerceptionComponent->ForgetAll();

	
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
	
	
}



void AMechPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMechPlayerController, TeamID);
	
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

		if (MapWidgetClass)
		{
			if (!MapWidget)
			{
				MapWidget = CreateWidget<UUserWidget>(this, MapWidgetClass);
				if (!MapWidget)return;
				MapWidget->AddToViewport();
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



void AMechPlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	

}


void AMechPlayerController::SenseStuff(AActor* Actor, FAIStimulus Stimulus)
{
	
	if (AMechGameCharacter *SensedCharacter = Cast<AMechGameCharacter>(Actor))
	{
		AMechGameCharacter * PlayerCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (PlayerCharacter && SensedCharacter != PlayerCharacter)
		{
			

		}
	}
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

