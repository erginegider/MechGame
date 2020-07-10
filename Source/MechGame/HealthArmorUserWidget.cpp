// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthArmorUserWidget.h"
#include "Components/ProgressBar.h"
#include "GameplayEffectTypes.h"
#include "MechGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MechAttributeSet.h"
#include "MechGameCharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"


UMaterialInstanceDynamic * UHealthArmorUserWidget::GetMaterialInstanceDynamic(UMaterialInterface * MaterialInterface, UImage& ImageWidget)
{

	UMaterialInstanceDynamic *MatInst;

	MatInst = UMaterialInstanceDynamic::Create(MaterialInterface, this);

	FSlateBrush NewBrush;
	NewBrush.SetImageSize(FVector2D(80.0f, 80.0f));

	NewBrush.SetResourceObject(MatInst);
	ImageWidget.SetBrush(NewBrush);


	return MatInst;

}

void UHealthArmorUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MechGameCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (MechGameCharacter)
	{
		AttributeSet = MechGameCharacter->MechAttributeSet;
		MaxHealthAttribute = AttributeSet->GetMaxHealthAttribute();
		MaxArmorAttribute = AttributeSet->GetMaxArmorAttribute();
		AbilitySystemComponent = MechGameCharacter->GetAbilitySystemComponent();
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack1"))).AddUObject(this,&UHealthArmorUserWidget::GameplayTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack2"))).AddUObject(this, &UHealthArmorUserWidget::GameplayTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense1"))).AddUObject(this, &UHealthArmorUserWidget::GameplayTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense2"))).AddUObject(this, &UHealthArmorUserWidget::GameplayTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Neutral1"))).AddUObject(this, &UHealthArmorUserWidget::GameplayTagChanged);
		

	}


	
	
	/*Attack1_Mat_Inst_Dyn = UMaterialInstanceDynamic::Create(Attack1_4x_Mat_Inst, this);

	FSlateBrush NewBrush;
	NewBrush.SetImageSize(FVector2D(80.0f, 80.0f));
	
	NewBrush.SetResourceObject(Attack1_Mat_Inst_Dyn);
	AttackSkillButton1->SetBrush(NewBrush);*/
	Attack1_Mat_Inst_Dyn = GetMaterialInstanceDynamic(Attack1_4x_Mat_Inst, *AttackSkillButton1);
	Attack2_Mat_Inst_Dyn = GetMaterialInstanceDynamic(Attack1_4x_Mat_Inst, *AttackSkillButton_1Overlay);
	Defense1_Mat_Inst_Dyn = GetMaterialInstanceDynamic(Attack1_4x_Mat_Inst, *DefenseSkillButton_2Overlay);
	Defense2_Mat_Inst_Dyn = GetMaterialInstanceDynamic(Attack1_4x_Mat_Inst, *DefenseSkillButton_3Overlay);
	Neutral1_Mat_Inst_Dyn = GetMaterialInstanceDynamic(Attack1_4x_Mat_Inst, *NeutralSkillButton_0Overlay);


	/*if (Attack1_Mat_Inst_Dyn)
	{
		UE_LOG( LogTemp, Warning, TEXT("  MATERIAL IS SET ") );
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("  NOT MATERIAL IS NOT SET "));
	}*/
	
	if (AbilitySystemComponent)
	{
		HealthBarProgressBar->SetPercent(MaxHealthAttribute.GetNumericValue(AttributeSet));
		ArmorProgressBar->SetPercent(MaxArmorAttribute.GetNumericValue(AttributeSet));
	}
	
}


void UHealthArmorUserWidget::GameplayTagChanged(const FGameplayTag GameplayTag, int32 Count)
{
	UE_LOG(LogTemp, Warning, TEXT("TAG CHANGED"));

		
	if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack1")))
	{
		if (Count == 0)
		{
			AttackSkillButton1->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			AttackSkillButton1->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack2")))
	{
		if (Count == 0)
		{
			AttackSkillButton_1Overlay->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			AttackSkillButton_1Overlay->SetVisibility(ESlateVisibility::Visible);
		}

	}
	else if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense1")))
	{
		if (Count == 0)
		{
			DefenseSkillButton_2Overlay->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			DefenseSkillButton_2Overlay->SetVisibility(ESlateVisibility::Visible);
		}

	}
	else if (GameplayTag == FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense2")))
	{
		if (Count == 0)
		{
			DefenseSkillButton_3Overlay->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			DefenseSkillButton_3Overlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if(GameplayTag == FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Neutral1")))
	{
		if (Count == 0)
		{
			NeutralSkillButton_0Overlay->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			NeutralSkillButton_0Overlay->SetVisibility(ESlateVisibility::Visible);
		}

	}

}


void UHealthArmorUserWidget::UpdateHealthBar(const FOnAttributeChangeData& Value)
{
	

		float MaxHealth =MaxHealthAttribute.GetNumericValue(AttributeSet);

		float NewHealthRatio = Value.NewValue / MaxHealth;
		

		//UE_LOG(LogTemp, Warning, TEXT("We have an update : Coming Value: %f      MaxHealth: %f "), Value.NewValue , MaxHealth);

		if (HealthBarProgressBar)
		{
			HealthBarProgressBar->SetPercent(NewHealthRatio);
		}
			
	
}



void UHealthArmorUserWidget::UpdateArmorBar(const FOnAttributeChangeData& Value)
{


	float MaxArmor = MaxArmorAttribute.GetNumericValue(AttributeSet);

	float NewArmorRatio = Value.NewValue / MaxArmor;


	//UE_LOG(LogTemp, Warning, TEXT("We have an update : Coming Armor Value: %f      MaxArmor: %f "), Value.NewValue, MaxArmor);

	

	if (ArmorProgressBar)
	{
		//ArmorProgressBar

		ArmorProgressBar->SetPercent(NewArmorRatio);

	}


}

void UHealthArmorUserWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick( MyGeometry, InDeltaTime );

	FGameplayTagContainer Attack1TagContainer(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack1")));
	FGameplayTagContainer Attack2TagContainer(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack2")));
	FGameplayTagContainer Defense1TagContainer(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense1")));
	FGameplayTagContainer Defense2TagContainer(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense2")));
	FGameplayTagContainer Neutral1TagContainer(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Neutral1")));

	float TimeRemaining;
	float CooldownDuration;
	if (!GetWorld()->IsInSeamlessTravel())
	{
		if (AbilitySystemComponent)
		{

			if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack1"))))
			{
				if (GetCooldownRemainingForTag(Attack1TagContainer, TimeRemaining, CooldownDuration) > 0.0f)
				{

					if (Attack1_Mat_Inst_Dyn)
					{
						//UE_LOG(LogTemp, Warning, TEXT("MatInst Name ....:%s "), *Attack1_Mat_Inst_Dyn->GetName());
						Attack1_Mat_Inst_Dyn->SetScalarParameterValue("Progress", TimeRemaining / CooldownDuration);
					}

				}
			}

			if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Attack2"))))
			{
				if (GetCooldownRemainingForTag(Attack2TagContainer, TimeRemaining, CooldownDuration) > 0.0f)
				{

					if (Attack2_Mat_Inst_Dyn)
					{
						//UE_LOG(LogTemp, Warning, TEXT("MatInst Name ....:%s "), *Attack1_Mat_Inst_Dyn->GetName());
						Attack2_Mat_Inst_Dyn->SetScalarParameterValue("Progress", TimeRemaining / CooldownDuration);
					}

				}
			}

			if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense1"))))
			{
				if (GetCooldownRemainingForTag(Defense1TagContainer, TimeRemaining, CooldownDuration) > 0.0f)
				{

					if (Defense1_Mat_Inst_Dyn)
					{
						//UE_LOG(LogTemp, Warning, TEXT("MatInst Name ....:%s "), *Attack1_Mat_Inst_Dyn->GetName());
						Defense1_Mat_Inst_Dyn->SetScalarParameterValue("Progress", TimeRemaining / CooldownDuration);
					}

				}
			}

			if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Defense2"))))
			{
				if (GetCooldownRemainingForTag(Defense2TagContainer, TimeRemaining, CooldownDuration) > 0.0f)
				{

					if (Defense2_Mat_Inst_Dyn)
					{
						//UE_LOG(LogTemp, Warning, TEXT("MatInst Name ....:%s "), *Attack1_Mat_Inst_Dyn->GetName());
						Defense2_Mat_Inst_Dyn->SetScalarParameterValue("Progress", TimeRemaining / CooldownDuration);
					}

				}
			}

			if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffectID.CoolDown.Neutral1"))))
			{
				if (GetCooldownRemainingForTag(Neutral1TagContainer, TimeRemaining, CooldownDuration) > 0.0f)
				{

					if (Neutral1_Mat_Inst_Dyn)
					{
						//UE_LOG(LogTemp, Warning, TEXT("MatInst Name ....:%s "), *Attack1_Mat_Inst_Dyn->GetName());
						Neutral1_Mat_Inst_Dyn->SetScalarParameterValue("Progress", TimeRemaining / CooldownDuration);
					}

				}

			}


		}

	}
	//AttackSkillButton_0Overlay->GetDynamicMaterial
	
	
	

}


float UHealthArmorUserWidget::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return TimeRemaining;
		}
	}

	return false;
}

