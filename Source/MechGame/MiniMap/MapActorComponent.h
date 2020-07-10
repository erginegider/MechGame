// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHGAME_API UMapActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMapActorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Component")
	class UTexture2D *ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Component")
	bool IsStatic;

protected:

	UFUNCTION()
	void SetUpMiniMap();
	// Called when the game starts
	virtual void BeginPlay() override;

	class UMiniMapUserWidget *MiniMapWidget;


	FTimerHandle SetupMiniMapHandle;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
		
};
