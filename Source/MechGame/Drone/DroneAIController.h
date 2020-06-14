// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "DroneAIController.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API ADroneAIController : public AAIController
{
	GENERATED_BODY()

	ADroneAIController();

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	class UAIPerceptionComponent * DronePerceptionComponent;*/
	class UAISenseConfig_Sight *SightConfig;
public:
	UFUNCTION()
	void SenseStuff(AActor* Actor, FAIStimulus Stimulus);

	bool BeginPlaying = false;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintNativeEvent,Category = "Callback")
	void UpdateTargetActor(AMechGameCharacter *TargetCharacter) const;


	//UFUNCTION(BlueprintCallable)
	//int32 GetTeamID();
};
