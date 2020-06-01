// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarUserWidget.h"
#include "Components/TextBlock.h"
#include "MechGameCharacter.h"
#include "GameplayEffectTypes.h"



void UHealthBarUserWidget::NativeConstruct()
{
	Super::NativeConstruct();	
}


void UHealthBarUserWidget::UpdateHealth(const FOnAttributeChangeData& NewHealth)
{
	float NewHealthValue = NewHealth.NewValue;
	FText Health = FText::FromString(FString::SanitizeFloat(NewHealthValue));	
	
	HealthBar->SetText(Health);
}