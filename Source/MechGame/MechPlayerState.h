// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MechPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class MECHGAME_API AMechPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Preplan")
	TSubclassOf<class AMechGameCharacter>  SelectedPlayer;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Preplan")
	bool isCharacterSet;

	virtual void CopyProperties(class APlayerState* PlayerState) override;

	AMechPlayerState();
};
