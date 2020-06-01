// Fill out your copyright notice in the Description page of Project Settings.


#include "MechAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "MechGameCharacter.h"
#include "MechPlayerController.h"
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