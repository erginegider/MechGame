// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MECHGAME_API UReticleUserWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	class AMechGameCharacter *MechGameCharacter;

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox *SizeBox_0 = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox *SizeBox_1 = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox *SizeBox_2 = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox *SizeBox_3 = nullptr;


	

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage *Reticle4_1 = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage *Reticle4_2 = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage *Reticle4_3 = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage *Reticle4_4 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Opening Amount")
	float OpeningAmount=25;

	float MaxCharacterSpeed = 500;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
