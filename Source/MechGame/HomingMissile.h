// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingMissile.generated.h"

UCLASS()
class MECHGAME_API AHomingMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingMissile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MissileMesh")
	class UStaticMeshComponent *MissileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MissileMesh")
	class UProjectileMovementComponent *ProjectileComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MissileMesh")
	class UCapsuleComponent *CapsuleComponent;

protected:

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect", meta=(ExposeOnSpawn="true"))
	TSubclassOf<class UGAmeplayEffect> GameplayEffect;

	UFUNCTION()
	void ActorBeginOverlap(AActor * OverlappedActor, AActor * OtherActor);
};
