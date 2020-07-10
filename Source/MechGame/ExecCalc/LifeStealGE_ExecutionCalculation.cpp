// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeStealGE_ExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "../MechAttributeSet.h"
#include "../MechGameCharacter.h"
struct LifeStealStatics
{
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	
	LifeStealStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMechAttributeSet, Health, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMechAttributeSet, Damage, Target, false);
	
	}


};

static LifeStealStatics& LifeSteal()
{
	static LifeStealStatics lfSteal;

	return lfSteal;
}

ULifeStealGE_ExecutionCalculation::ULifeStealGE_ExecutionCalculation()
{
	RelevantAttributesToCapture.Add(LifeSteal().HealthDef);
	RelevantAttributesToCapture.Add(LifeSteal().DamageDef);
	
}

void ULifeStealGE_ExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	UAbilitySystemComponent *TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent *SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor *SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor *TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(LifeSteal().HealthDef, EvaluationParameters, Health);
	Health = FMath::Max<float>(Health, 0.0f);	

	float Damage =FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("GameplayEffectMessage.DamageAmount")), false, -1.0f),0.0f);
	if (Damage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(LifeSteal().DamageProperty, EGameplayModOp::Additive, Damage));
	}
		
	UGameplayEffect *HealingEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("HealSelfEffect")));
	HealingEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	int32 TotalModifiers = HealingEffect->Modifiers.Num();
	HealingEffect->Modifiers.SetNum(TotalModifiers+1);
	FGameplayModifierInfo& Info = HealingEffect->Modifiers[TotalModifiers];
	Info.ModifierMagnitude = FScalableFloat(Damage);
	Info.ModifierOp = EGameplayModOp::Additive;	
	Info.Attribute = UMechAttributeSet::GetHealthAttribute();
	SourceAbilitySystemComponent->ApplyGameplayEffectToSelf(HealingEffect, 1.0f, SourceAbilitySystemComponent->MakeEffectContext());

	//UE_LOG(LogTemp, Warning, TEXT("ExecCallc Damage....:%f      Health....:%f    SourceActorName.....:%s"), Damage, Health, *(SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get()->GetName()));
}
