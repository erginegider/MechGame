// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "../MechGameCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

 
// Sets default values
ATrapActor::ATrapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCompoent"));
	BoxComponent->SetupAttachment(RootComponent);
	
	

	BoxComponent->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);
	BoxComponent->SetGenerateOverlapEvents(true);
	OnActorBeginOverlap.AddDynamic(this, &ATrapActor::ActorBeginOverlap);
}

// Called when the game starts or when spawned
void ATrapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame


void ATrapActor::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("This is It"));

	AMechGameCharacter *OverlappingCharacter = Cast<AMechGameCharacter>(OtherActor);

	if (OverlappingCharacter)
	{
		UAbilitySystemComponent *AbilitySystemComponent = OverlappingCharacter->GetAbilitySystemComponent();

		if (AbilitySystemComponent)
		{
			if (GameplayEffect)
			{
				if (GetTeamAttitudeTowards(OverlappingCharacter) != ETeamAttitude::Friendly)
				{
					FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
					FGameplayEffectSpecHandle GameplayEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, Context);
					AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data);
				}
				
			}
			

		}
	}


}


ETeamAttitude::Type ATrapActor::GetTeamAttitudeTowards(const AMechGameCharacter *Other) const
{
	
		if (const IGenericTeamAgentInterface *TeamAgentInterface = Cast<IGenericTeamAgentInterface>(Other))
		{

			return FGenericTeamId::GetAttitude(GetGenericTeamId(), Other->GetGenericTeamId());
		}
	
	return ETeamAttitude::Hostile;
}