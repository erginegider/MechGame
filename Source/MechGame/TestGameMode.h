// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnClasses")
	TMap<int32, TSubclassOf<APawn>> SpawnPawnMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnClasses")
	TArray<FString>  PawnStrings;

	ATestGameMode();
	

	virtual void BeginPlay();

	UFUNCTION()
	void UpdatePlayerList();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
