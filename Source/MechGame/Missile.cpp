// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameplayEffect.h"
#include "MechGameCharacter.h"
#include "AbilitySystemComponent.h"

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovementComponent->InitialSpeed = 7000.0f;

	CollissionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColissionCapsule"));
	CollissionCapsule->SetupAttachment(RootComponent);
	CollissionCapsule->SetRelativeRotation(FRotator(0.0f,90.0f,0.0f));


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	DamageToApply = 1.0f;

	CollissionCapsule->IgnoreActorWhenMoving(this->GetOwner(), true);
	//CollissionCapsule->SetCollisionProfileName()
	CollissionCapsule->OnComponentBeginOverlap.AddDynamic(this,&AMissile::OnCapsuleBeginOverlap);
	
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	
}
void AMissile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{

		AMechGameCharacter *OtherCharacter = Cast<AMechGameCharacter>(OtherActor);
		if (OtherCharacter)
		{
			UAbilitySystemComponent *OtherAbilitySystemComponent = OtherCharacter->GetAbilitySystemComponent();
			if (OtherAbilitySystemComponent)
			{
				if (DamageEffect)
				{
					FGameplayEffectContextHandle Context = OtherAbilitySystemComponent->MakeEffectContext();
					Context.AddHitResult(SweepResult);
					Context.AddOrigin(SweepResult.Location);
					TArray<TWeakObjectPtr<AActor>> ActorList;
					ActorList.Add(OtherActor);
					Context.AddActors(ActorList);
					Context.AddSourceObject(this);
					FGameplayEffectSpecHandle DamageEffectSpecHandle= OtherAbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.0, Context);
					
					OtherAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
				}
			}

		}
		
	}
	
}





//// Called every frame
//void AMissile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

