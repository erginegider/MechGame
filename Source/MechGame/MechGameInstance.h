// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Containers/Map.h"
#include "Engine/Player.h"
#include "MechGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API UMechGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	TSubclassOf<class AMechGameCharacter> DefaultPawn;


	virtual void Init();
	
};
