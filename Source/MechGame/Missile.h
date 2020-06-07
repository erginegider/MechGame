// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

UCLASS()
class MECHGAME_API AMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class UProjectileMovementComponent *ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class UStaticMeshComponent *StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class UCapsuleComponent *CollissionCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class UArrowComponent   *ArrowComponent;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Campfire)
	UParticleSystem* FireParticle;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Campfire)
	class USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly,Category = "Projectile")
	TSubclassOf<class UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere,Category="Projectile");
	float DamageToApply;


	// Called every frame
	/*virtual void Tick(float DeltaTime) override;*/

};
