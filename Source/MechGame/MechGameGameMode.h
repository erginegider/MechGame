// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Containers/Map.h"
#include "MechGameGameMode.generated.h"

UCLASS(minimalapi)
class AMechGameGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
public:
	AMechGameGameMode();

	void ServerTravel();	

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};



