// Fill out your copyright notice in the Description page of Project Settings.


#include "StunActor.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "../MechGameCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStunActor::AStunActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	SphereComponent->SetSphereRadius(1200.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);
	SphereComponent->SetGenerateOverlapEvents(true);


	OnActorBeginOverlap.AddDynamic(this, &AStunActor::ActorBeginOverlap);
}

// Called when the game starts or when spawned
void AStunActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStunActor::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT(" we are stun !! "));

	AMechGameCharacter *OverlappingCharacter = Cast<AMechGameCharacter>(OtherActor);

	if (OverlappingCharacter)
	{
		UAbilitySystemComponent *AbilitySystemComponent = OverlappingCharacter->GetAbilitySystemComponent();
		AMechGameCharacter *PlayerCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if(PlayerCharacter && PlayerCharacter != OverlappingCharacter )
		if (AbilitySystemComponent)
		{
			if (GameplayEffect)
			{
				
					FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
					FGameplayEffectSpecHandle GameplayEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, Context);
					AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data);
				

			}


		}
	}

}


ETeamAttitude::Type AStunActor::GetTeamAttitudeTowards(const AMechGameCharacter *Other) const
{

	if (const IGenericTeamAgentInterface *TeamAgentInterface = Cast<IGenericTeamAgentInterface>(Other))
	{

		return FGenericTeamId::GetAttitude(GetGenericTeamId(), Other->GetGenericTeamId());
	}

	return ETeamAttitude::Hostile;
}