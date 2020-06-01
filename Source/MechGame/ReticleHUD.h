// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ReticleHUD.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API AReticleHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	UTexture2D *ReticleTexture;


	virtual void DrawHUD() override;
};
