// Fill out your copyright notice in the Description page of Project Settings.


#include "MechAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "MechGameCharacter.h"
#include "MechPlayerController.h"
#include "GameplayEffectExtension.h"
#include "Kismet/GameplayStatics.h"



UMechAttributeSet::UMechAttributeSet()
{

}

void UMechAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMechAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMechAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMechAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMechAttributeSet, MaxArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMechAttributeSet, DamageRatio, COND_None, REPNOTIFY_Always);
}

void UMechAttributeSet::OnRep_Health(const FGameplayAttributeData & OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMechAttributeSet, Health, OldHealth);
}


void UMechAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData & OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMechAttributeSet, MaxHealth, OldMaxHealth);
}


void UMechAttributeSet::OnRep_Armor(const FGameplayAttributeData & OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMechAttributeSet, Armor, OldArmor);
}

void UMechAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData & OldMaxArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMechAttributeSet, MaxArmor, OldMaxArmor);

}


void UMechAttributeSet::OnRep_DamageRatio(const FGameplayAttributeData & OldDamageRatio)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMechAttributeSet, DamageRatio, OldDamageRatio);
}

void UMechAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent *Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	AActor *TargetActor = nullptr;
	AController *TargetController = nullptr;
	AMechGameCharacter *TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AMechGameCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		AActor *SourceActor = nullptr;
		AController *SourceController = nullptr;
		AMechGameCharacter *SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			SourceCharacter = Cast<AMechGameCharacter>(SourceActor);
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				APawn * Pawn = Cast<APawn>(SourceActor);
				if (Pawn)
				{
					SourceController = Pawn->GetController();
				}
			}
			if (SourceController && SourceCharacter==nullptr)
			{
				SourceCharacter = Cast<AMechGameCharacter>(SourceController->GetPawn());

			}

			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}

			FHitResult HitResult;
			if (Context.GetHitResult())
			{
				HitResult = *Context.GetHitResult();
			}

			float localdamage = GetDamage();
			float localhealth = GetHealth();
			float localarmor = GetArmor();

			SetDamage(0.0f);
			//UE_LOG(LogTemp, Warning, TEXT("Damage RAW...: %f      Damage Ratio......: %f       Damage Final....: %f"),localdamage,GetDamageRatio(),localdamage*GetDamageRatio());
			localdamage = localdamage * GetDamageRatio();

			if (localdamage <= localarmor)
			{
				localarmor = localarmor - localdamage;
			}
			else
			{
				float damageportion = localdamage - localarmor;
				localarmor = 0;
				localhealth = localhealth - damageportion;

			}

			
			float finalhealth = localhealth;

			finalhealth = FMath::Clamp<float>(finalhealth, 0, GetMaxHealth());
			//UE_LOG(LogTemp, Warning, TEXT("Final Health...: %f   Final Armor.....:%f "), finalhealth, localarmor);

			SetHealth(finalhealth);
			SetArmor(localarmor);

			SourceCharacter->OnPawnHealthChanged.Broadcast(finalhealth);
		}
	} // If Damage Close ??
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}


}

void UMechAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	
	Super::PreAttributeChange(Attribute, NewValue);
	
	AMechGameCharacter *OwningCharacter=Cast<AMechGameCharacter>(GetOwningActor());
	if (OwningCharacter)
	{
		
		
			AMechPlayerController *OwningPlayerController = Cast<AMechPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

			if (OwningPlayerController)
			{
				if (Attribute == GetHealthAttribute())
				{					
					
					if (OwningCharacter->OnPawnHealthChanged.IsBound())
					{
						NewValue = FMath::Clamp<float>(NewValue, 0, GetMaxHealth());
						OwningCharacter->OnPawnHealthChanged.Broadcast(NewValue);
						
					}				

				}


				if (Attribute == GetArmorAttribute())
				{
					if (OwningCharacter->OnPawnArmorChanged.IsBound())
					{
						OwningCharacter->OnPawnArmorChanged.Broadcast(GetArmor());
					}
				}
			}
			else UE_LOG(LogAbilitySystem, Warning, TEXT("No Owning Player Controller"));

			
		
	}
	else UE_LOG(LogAbilitySystem, Warning, TEXT("No Owning Character"));	
	
	
}

void UMechAttributeSet::PreAttributeBaseChange(const FGameplayAttribute & Attribute, float & NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{

		//UE_LOG(LogAbilitySystem, Warning, TEXT("%f    %f"), NewValue, GetMaxHealth());
		//NewValue = FMath::Clamp<float>(NewValue, 0, GetMaxHealth());

			


			//OwningCharacter->OnPawnHealthChanged.Broadcast(NewValue);


	}
}
