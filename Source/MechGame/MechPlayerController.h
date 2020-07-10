// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MechPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerHealthChanged, float, NewHealth);
/**
 * 
 */
UCLASS()
class MECHGAME_API AMechPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Replicated,Category="Team")
	FGenericTeamId TeamID;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIPerception")
	class UAIPerceptionComponent *PerceptionComponent;

	class UAISenseConfig_Sight *SightConfig;

public:


	AMechPlayerController();	

	

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
	
	FOnControllerHealthChanged OnHealthChanged;

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void SenseStuff(AActor * Actor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> PlayerReticleClass;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> MapWidgetClass;

	
	class UUserWidget *PlayerWidget;

	
	class UUserWidget *PlayerReticle;
	   	 
	
	class UUserWidget *MapWidget;

	

	UFUNCTION()
	virtual void AcknowledgePossession(class APawn* P) override;

	UFUNCTION()
	virtual void ClientRestart_Implementation(class APawn* NewPawn) override;

	UFUNCTION()
	virtual void OnUnPossess() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetupTeam(AMechGameCharacter *SetPawn);

	

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor & Other) const override;
};
