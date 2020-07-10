// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "GenericTeamAgentInterface.h"
#include "MiniMapUserWidget.generated.h"


USTRUCT()
struct MECHGAME_API FMiniMapInfo
{
	GENERATED_USTRUCT_BODY()
	FMiniMapInfo()
	{
		ItemIcon = nullptr;
		Item = nullptr;
		IsStatic = true;
	}

	class UTexture2D *ItemIcon;

	class AActor *Item;

	class UIconWidget *Widget;

	bool IsStatic;

	ETeamAttitude::Type TeamAttitude;
	

};
/**
 * 
 */
UCLASS()
class MECHGAME_API UMiniMapUserWidget : public UUserWidget
{
	GENERATED_BODY()
			
public:

	TArray<FMiniMapInfo> MiniMapInfo;

	UPROPERTY(EditAnywhere, Category = "NewItemWidget")
	TSubclassOf<class UIconWidget> NewItemClass;

	UPROPERTY(EditAnywhere, Category = "NewItemWidget")
	class UMaterialParameterCollection *MaterialProperties;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UCanvasPanel  *MiniMapCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage  *MiniMap;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UIconWidget *MapIcon;

	FVector2D MapSize;


	class UMaterialParameterCollectionInstance *MaterialParameterCollectionInst;

	float MapDimension;
	float ratio;


	class AMechPlayerController * MechPlayerController;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

	virtual void AddNewItem(FMiniMapInfo& Item);
};
