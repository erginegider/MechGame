// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "HealthArmorUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API UHealthArmorUserWidget : public UUserWidget
{
	GENERATED_BODY()
			   
	

	class AMechGameCharacter *MechGameCharacter;


	class UAbilitySystemComponent *AbilitySystemComponent;

	class UMechAttributeSet *AttributeSet;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface *Attack1_4x_Mat_Inst;
	UMaterialInstanceDynamic *Attack1_Mat_Inst_Dyn;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface *Attack2_4x_Mat_Inst;
	UMaterialInstanceDynamic *Attack2_Mat_Inst_Dyn;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface *Defense1_4x_Mat_Inst;
	UMaterialInstanceDynamic *Defense1_Mat_Inst_Dyn;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface *Defense2_4x_Mat_Inst;
	UMaterialInstanceDynamic *Defense2_Mat_Inst_Dyn;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface *Neutral1_4x_Mat_Inst;
	UMaterialInstanceDynamic *Neutral1_Mat_Inst_Dyn;

	

	FGameplayAttribute MaxHealthAttribute;
	FGameplayAttribute MaxArmorAttribute;

public:

	virtual	void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar *HealthBarProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage  *AttackSkillButton1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage  *AttackSkillButton_1Overlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage  *DefenseSkillButton_2Overlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage  *DefenseSkillButton_3Overlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage  *NeutralSkillButton_0Overlay;

	

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar *ArmorProgressBar;

	UFUNCTION()
	void GameplayTagChanged(const FGameplayTag GameplayTag, int32 Count);

	void UpdateHealthBar(const FOnAttributeChangeData& Value);
	void UpdateArmorBar(const FOnAttributeChangeData & Value);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION(BlueprintCallable, Category = "GameplayAbilities")
	float GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration);


	UMaterialInstanceDynamic * GetMaterialInstanceDynamic(UMaterialInterface * MaterialInterface, UImage& Widget);
};
