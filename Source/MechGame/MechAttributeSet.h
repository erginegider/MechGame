// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MechAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MECHGAME_API UMechAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMechAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(BlueprintReadOnly, Category = "AbilitySets", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMechAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "AbilitySets", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMechAttributeSet, MaxHealth)
	

	UPROPERTY(BlueprintReadOnly, Category = "AbilitySets", ReplicatedUsing = OnRep_DamageRatio)
	FGameplayAttributeData DamageRatio;
	ATTRIBUTE_ACCESSORS(UMechAttributeSet, DamageRatio)


	UPROPERTY(BlueprintReadOnly, Category = "AbilitySets", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UMechAttributeSet, Armor)


	UPROPERTY(BlueprintReadOnly, Category = "AbilitySets", ReplicatedUsing = OnRep_MaxArmor)
	FGameplayAttributeData MaxArmor;
	ATTRIBUTE_ACCESSORS(UMechAttributeSet, MaxArmor)

	UPROPERTY(BlueprintReadOnly, Category = "AbilitySets")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UMechAttributeSet, Damage)

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData & OldArmor);

	UFUNCTION()
	virtual void OnRep_MaxArmor(const FGameplayAttributeData & OldMaxArmor);

	UFUNCTION()
	virtual void OnRep_DamageRatio(const FGameplayAttributeData & OldDamageRatio);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
};
