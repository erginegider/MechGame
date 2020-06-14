// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"

UCLASS()
class MECHGAME_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite,Category = "Team", meta=(ExposeOnSpawn="true"))
	int32 TeamId;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE int32 GetTeamId() { return TeamId; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
