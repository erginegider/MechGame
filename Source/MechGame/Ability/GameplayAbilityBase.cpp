// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

TArray<FActiveGameplayEffectHandle> UGameplayAbilityBase::ApplyGameplayEffectsToTargetWithTags(FGameplayTag Tags, FGameplayAbilityTargetDataHandle Target, float Level)
{
	TArray<FActiveGameplayEffectHandle> ReturnEffects;
	FGameplayEffectContainer EffectList= *EffectsList.GameplayEffectsMap.Find(Tags);

	//EffectList=
	for (auto& Effect :EffectList.EffectsToApply)
	{
		FGameplayEffectSpecHandle OutgoingEffectSpec = MakeOutgoingGameplayEffectSpec(Effect,Level);
		ReturnEffects.Append(K2_ApplyGameplayEffectSpecToTarget(OutgoingEffectSpec,Target));
	}
	
	return ReturnEffects;
}

void UGameplayAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	if (ShouldActivateWhenOwned)
	{
		bool IsActivated= ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle,false);
	}
}

