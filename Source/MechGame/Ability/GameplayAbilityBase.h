// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "FEffectContainer.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffects")
	FGameplayEffectContainerforTarget  EffectsList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Passive Activation")
	bool ShouldActivateWhenOwned = false;

	UFUNCTION(BlueprintCallable)
	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectsToTargetWithTags(FGameplayTag Tags,FGameplayAbilityTargetDataHandle Target, float Level=1.0f);


	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
