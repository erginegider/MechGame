// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "StunActor.generated.h"

UCLASS()
class MECHGAME_API AStunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStunActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StunActor")
	class USphereComponent *SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	

	UFUNCTION()
	void ActorBeginOverlap(AActor * OverlappedActor, AActor * OtherActor);

	ETeamAttitude::Type GetTeamAttitudeTowards(const class AMechGameCharacter *Other) const;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect")
	TSubclassOf<class UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "GameplayEffect")
	FGenericTeamId TeamId;

	FGenericTeamId GetGenericTeamId() const { return TeamId; }
};
