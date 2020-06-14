// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
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
public:

	UPROPERTY(Replicated,ReplicatedUsing=OnRep_DefaultPawn,BlueprintReadWrite,Category="MyPawnClass")
	TSubclassOf<class AMechGameCharacter> DefaultPawn;

	AMechPlayerController();

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor & Other) const override;



	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Clicked(AMechGameCharacter *selectedpawn);
	void Server_Clicked_Implementation(AMechGameCharacter *selectedpawn);
	bool Server_Clicked_Validate(AMechGameCharacter *selectedpawn);

	FOnControllerHealthChanged OnHealthChanged;

	UFUNCTION()
	void OnRep_DefaultPawn();

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	TSubclassOf<APawn> GetDefaultPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UUserWidget *PlayerWidget;

	UFUNCTION()
	virtual void AcknowledgePossession(class APawn* P) override;

	UFUNCTION()
	virtual void ClientRestart_Implementation(class APawn* NewPawn) override;

	UFUNCTION()
	virtual void OnUnPossess() override;

	

};
