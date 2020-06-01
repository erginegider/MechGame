// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "HealthBarUserWidget.generated.h"
//D:\UNREALENGINE\UE_4.25\Engine\Plugins\Runtime\GameplayAbilities\Source\GameplayAbilities\Public\GameplayEffectTypes.h
/**
 * 
 */
UCLASS()
class MECHGAME_API UHealthBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	class UTextBlock *HealthBar;
	

	virtual void NativeConstruct() override;

	
	void UpdateHealth(const  FOnAttributeChangeData& NewHealth);
};
