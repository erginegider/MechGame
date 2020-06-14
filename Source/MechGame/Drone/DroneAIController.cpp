// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAIController.h"
#include "DronePawn.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../MechGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../MechPlayerController.h"

ADroneAIController::ADroneAIController()
{

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionCompoent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));


	BeginPlaying = false;

}


void ADroneAIController::SenseStuff(AActor* Actor, FAIStimulus Stimulus)
{
	
	if (AMechGameCharacter *SensedCharacter = Cast<AMechGameCharacter>(Actor))
	{
		AMechGameCharacter * PlayerCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		
		if (PlayerCharacter && SensedCharacter != PlayerCharacter)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Sense Something"));	
			
			UpdateTargetActor(SensedCharacter);

		}
	}
}

ETeamAttitude::Type ADroneAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const AMechGameCharacter *OtherPawn = Cast<AMechGameCharacter>(&Other))
	{
		if (const IGenericTeamAgentInterface *TeamAgentInterface = Cast<IGenericTeamAgentInterface>(OtherPawn))
		{
			AMechPlayerController *PlayerController = Cast<AMechPlayerController>(OtherPawn->GetController());
			if (PlayerController && BeginPlaying)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Red, FString::Printf(TEXT("OtherController Id %d Drone Controller...:%d   Result %d "), PlayerController->GetGenericTeamId().GetId(),GetGenericTeamId().GetId(), (uint8)Super::GetTeamAttitudeTowards(*PlayerController)) );
				return Super::GetTeamAttitudeTowards(*PlayerController);

			}			
			
		}
	}

	return ETeamAttitude::Neutral;
}

void ADroneAIController::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Controller Begin"));
	
	

	if (ADronePawn  *myPawn = Cast<ADronePawn>(GetPawn()))
	{
		int32 TeamId = myPawn->GetTeamId();
		SetGenericTeamId(FGenericTeamId(TeamId));
	}
	BeginPlaying = true;
	PerceptionComponent->ForgetAll();
}

void ADroneAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	if (ADronePawn *myPawn = Cast<ADronePawn>(InPawn))
	{
		SetGenericTeamId(FGenericTeamId(myPawn->GetTeamId()));
	}


	
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ADroneAIController::SenseStuff);
	PerceptionComponent->ForgetAll();
	
}
//
//FGenericTeamId ADroneAIController::GetTeamID()
//{
//	return GetGenericTeamId();
//}

void ADroneAIController::UpdateTargetActor_Implementation(AMechGameCharacter * TargetCharacter) const
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Enemy Spotted"));
}
