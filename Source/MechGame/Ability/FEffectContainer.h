// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "FEffectContainer.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FGameplayEffectContainer 
{
	GENERATED_BODY()
public:
	FGameplayEffectContainer(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayEffects")
	TArray<TSubclassOf<class UGameplayEffect>> EffectsToApply;
};


USTRUCT(BlueprintType)
struct  FGameplayEffectContainerforTarget
{
	GENERATED_BODY()
public:
	FGameplayEffectContainerforTarget(){}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffects")
	TMap<FGameplayTag, FGameplayEffectContainer> GameplayEffectsMap;
};

