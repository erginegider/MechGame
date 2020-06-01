// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MechGameGameMode.generated.h"

UCLASS(minimalapi)
class AMechGameGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMechGameGameMode();

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};



