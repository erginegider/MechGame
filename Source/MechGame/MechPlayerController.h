// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MechPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerHealthChanged, float, NewHealth);
/**
 * 
 */
UCLASS()
class MECHGAME_API AMechPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	UPROPERTY(Replicated,ReplicatedUsing=OnRep_DefaultPawn,BlueprintReadWrite,Category="MyPawnClass")
	TSubclassOf<class AMechGameCharacter> DefaultPawn;

	AMechPlayerController();

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

};
