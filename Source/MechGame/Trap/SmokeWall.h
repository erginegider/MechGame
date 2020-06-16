// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmokeWall.generated.h"

UCLASS()
class MECHGAME_API ASmokeWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmokeWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxExtend")
	class UBoxComponent *BoxComponent;


public:	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleCount")
	int32 ParticleCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem  *ParticleEffect;

};
