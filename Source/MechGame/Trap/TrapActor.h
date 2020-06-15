// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "TrapActor.generated.h"

UCLASS()
class MECHGAME_API ATrapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxComponent")
	class UBoxComponent *BoxComponent;

public:	
	// Called every frame
	

	UPROPERTY(EditDefaultsOnly,Category = "GameplayEffect")
	TSubclassOf<class UGameplayEffect> GameplayEffect;

	UFUNCTION()
	void ActorBeginOverlap(AActor * OverlappedActor, AActor * OtherActor);

	ETeamAttitude::Type GetTeamAttitudeTowards(const class AMechGameCharacter *Other) const;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn="true"),Category = "GameplayEffect")
	FGenericTeamId TeamId;

	FGenericTeamId GetGenericTeamId() const { return TeamId; }

};
