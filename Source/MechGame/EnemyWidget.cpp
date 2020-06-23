// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidget.h"
#include "MechGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MechAttributeSet.h"
#include "Components/ProgressBar.h"

void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MechGameCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (MechGameCharacter)
	{
		AttributeSet = MechGameCharacter->MechAttributeSet;
		MaxHealthAttribute = AttributeSet->GetMaxHealthAttribute();
		MaxArmorAttribute = AttributeSet->GetMaxArmorAttribute();

	}



	EnemyHealthBarProgressBar->SetPercent(MaxHealthAttribute.GetNumericValue(AttributeSet));
	EnemyArmorBarProgressBar->SetPercent(MaxArmorAttribute.GetNumericValue(AttributeSet));
}



void UEnemyWidget::UpdateHealthBar(const FOnAttributeChangeData& Value)
{


	float MaxHealth = MaxHealthAttribute.GetNumericValue(AttributeSet);

	float NewHealthRatio = Value.NewValue / MaxHealth;


	UE_LOG(LogTemp, Warning, TEXT("We have an update : Coming Value: %f      MaxHealth: %f "), Value.NewValue, MaxHealth);

	if (EnemyHealthBarProgressBar)
	{
		EnemyHealthBarProgressBar->SetPercent(NewHealthRatio);
	}


}



void UEnemyWidget::UpdateArmorBar(const FOnAttributeChangeData& Value)
{


	float MaxArmor = MaxArmorAttribute.GetNumericValue(AttributeSet);

	float NewArmorRatio = Value.NewValue / MaxArmor;


	UE_LOG(LogTemp, Warning, TEXT("We have an update : Coming Armor Value: %f      MaxArmor: %f "), Value.NewValue, MaxArmor);



	if (EnemyArmorBarProgressBar)
	{
		//ArmorProgressBar

		EnemyArmorBarProgressBar->SetPercent(NewArmorRatio);

	}


}