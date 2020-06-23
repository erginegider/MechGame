// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	class AMechGameCharacter *MechGameCharacter;

	class UMechAttributeSet *AttributeSet;

	FGameplayAttribute MaxHealthAttribute;
	FGameplayAttribute MaxArmorAttribute;

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar *EnemyHealthBarProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar *EnemyArmorBarProgressBar;
	

	void UpdateHealthBar(const FOnAttributeChangeData& Value);

	void UpdateArmorBar(const FOnAttributeChangeData& Value);
};
