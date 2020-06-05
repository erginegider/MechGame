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
}

void UMechAttributeSet::OnRep_Health(const FGameplayAttributeData & OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMechAttributeSet, Health, OldHealth);
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

			const float localdamage = GetDamage();
			SetDamage(0.0f);


			const float localhealth = GetHealth();
			UE_LOG(LogTemp, Warning, TEXT("AttributeSet localdamage....:%f      localhealth....:%f"), localdamage, localhealth);
			SetHealth(localhealth-localdamage);
			SourceCharacter->OnPawnHealthChanged.Broadcast(GetHealth());
		}
	} // If Damage Close ??


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
						
						OwningCharacter->OnPawnHealthChanged.Broadcast(GetHealth());
						
					}				

				}
			}
			else UE_LOG(LogAbilitySystem, Warning, TEXT("No Owning Player Controller"));

			
		
	}
	else UE_LOG(LogAbilitySystem, Warning, TEXT("No Owning Character"));	
	
	
}