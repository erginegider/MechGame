// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#define print(x)  GEngine->AddOnScreenDebugMessage(-1,8.0f,FColor::Green,TEXT(x))
#define prints(x)  GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,FString::Printf(TEXT("%s"),x))
#define printn(x)  GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,FString::Printf(TEXT("%i"),x))


UENUM(BlueprintType)
enum class EMechAbilityInput :uint8
{		
	UseAbilityFire UMETA(DisplayName = "Use Ability Fire"),
	UseAbilityMissileFire UMETA(DisplayName = "Use Ability Missile"),
	UseAbilityAttackFirst UMETA(DisplayName = "Use Ability Attack First"),
	UseAbilityAttackSecond UMETA(DisplayName = "Use Ability Attack Second"),
	UseAbilityDefenseFirst UMETA(DisplayName = "Use Ability Defense First"),
	UseAbilityDefenseSecond UMETA(DisplayName = "Use Ability Defense Second"),
	UseAbilityNeutral UMETA(DisplayName = "Use Ability Neutral"),
	UseAbilityCustom1 UMETA(DisplayName = "Use Ability Custom1"),
	UseAbilityCustom2 UMETA(DisplayName = "Use Ability Custom2"),
	UseAbilityCustom3 UMETA(DisplayName = "Use Ability Custom3"),
	UseAbilityCustom4 UMETA(DisplayName = "Use Ability Custom4"),
	UseAbilityCustom5 UMETA(DisplayName = "Use Ability Custom5"),
	ConfirmInput UMETA(DisplayName = "Confirm Input"),
	CancelInput UMETA(DisplayName = "Cancel Input"),
	
};

UENUM(BlueprintType)
enum class EAbilityKey:uint8
{
	FireKey UMETA(DisplayName = "Use Ability Fire Key"),
	MissileFireKey UMETA(DisplayName = "Use Ability Missile Fire Key"),
	AttackFirstKey UMETA(DisplayName = "Use Ability Attack First Key"),
	AttackSecondKey UMETA(DisplayName = "Use Ability Attack Second Key"),
	DefenseFirstKey UMETA(DisplayName = "Use Ability Defense First Key"),
	DefenseSecondKey UMETA(DisplayName = "Use Ability Defense Second Key"),
	NeutralKey UMETA(DisplayName = "Use Ability Neutral Key"),
	Custom1Key UMETA(DisplayName = "Use Ability Custom 1"),
	Custom2Key UMETA(DisplayName = "Use Ability Custom 2"),
	Custom3Key UMETA(DisplayName = "Use Ability Custom 3"),
	Custom4Key UMETA(DisplayName = "Use Ability Custom 4"),
	Custom5Key UMETA(DisplayName = "Use Ability Custom 5"),

};

inline uint8 GetTypeHash(const EAbilityKey A)
{
	return (uint8)A;
}