// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAIController.h"
#include "DronePawn.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../MechGameCharacter.h"
#include "Kismet/GameplayStatics.h"


ADroneAIController::ADroneAIController()
{

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionCompoent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ADroneAIController::SenseStuff);
	SetGenericTeamId(FGenericTeamId(1));

}


void ADroneAIController::SenseStuff(AActor* Actor, FAIStimulus Stimulus)
{
	
	if (AMechGameCharacter *SensedCharacter = Cast<AMechGameCharacter>(Actor))
	{
		AMechGameCharacter * PlayerCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		
		if (PlayerCharacter && SensedCharacter != PlayerCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Sense Something"));
			UpdateTargetActor(SensedCharacter);

		}
	}
}

ETeamAttitude::Type ADroneAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn *OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface *TeamAgentInterface = Cast<IGenericTeamAgentInterface>(OtherPawn))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
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
	
}

void ADroneAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	if (ADronePawn *myPawn = Cast<ADronePawn>(InPawn))
	{
		SetGenericTeamId(FGenericTeamId(myPawn->GetTeamId()));
	}

	
}

void ADroneAIController::UpdateTargetActor_Implementation(AMechGameCharacter * TargetCharacter) const
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Enemy Spotted"));
}
