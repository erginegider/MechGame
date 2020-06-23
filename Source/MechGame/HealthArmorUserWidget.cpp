// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthArmorUserWidget.h"
#include "Components/ProgressBar.h"
#include "GameplayEffectTypes.h"
#include "MechGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MechAttributeSet.h"
#include "MechGameCharacter.h"


void UHealthArmorUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MechGameCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (MechGameCharacter)
	{
		AttributeSet = MechGameCharacter->MechAttributeSet;
		MaxHealthAttribute = AttributeSet->GetMaxHealthAttribute();
		MaxArmorAttribute = AttributeSet->GetMaxArmorAttribute();
		
	}

	

	HealthBarProgressBar->SetPercent(MaxHealthAttribute.GetNumericValue(AttributeSet));
	ArmorProgressBar->SetPercent(MaxArmorAttribute.GetNumericValue(AttributeSet));
}

void UHealthArmorUserWidget::UpdateHealthBar(const FOnAttributeChangeData& Value)
{
	

		float MaxHealth =MaxHealthAttribute.GetNumericValue(AttributeSet);

		float NewHealthRatio = Value.NewValue / MaxHealth;
		

		UE_LOG(LogTemp, Warning, TEXT("We have an update : Coming Value: %f      MaxHealth: %f "), Value.NewValue , MaxHealth);

		if (HealthBarProgressBar)
		{
			HealthBarProgressBar->SetPercent(NewHealthRatio);
		}
			
	
}



void UHealthArmorUserWidget::UpdateArmorBar(const FOnAttributeChangeData& Value)
{


	float MaxArmor = MaxArmorAttribute.GetNumericValue(AttributeSet);

	float NewArmorRatio = Value.NewValue / MaxArmor;


	UE_LOG(LogTemp, Warning, TEXT("We have an update : Coming Armor Value: %f      MaxArmor: %f "), Value.NewValue, MaxArmor);

	

	if (ArmorProgressBar)
	{
		//ArmorProgressBar

		ArmorProgressBar->SetPercent(NewArmorRatio);

	}


}