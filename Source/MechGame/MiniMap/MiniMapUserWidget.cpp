// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapUserWidget.h"
#include "IconWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "../MechPlayerController.h"

void UMiniMapUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MechPlayerController = Cast<AMechPlayerController>( GetOwningPlayer() );
	
	if (MaterialProperties)
	{
		MaterialParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(MaterialProperties);
		if (MaterialParameterCollectionInst)
		{
			MaterialParameterCollectionInst->GetScalarParameterValue("Dimension", MapDimension);
			
		}
	}
		
}
	

void UMiniMapUserWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	APawn *OwningPlayerPawn = GetOwningPlayerPawn();
	if (OwningPlayerPawn)
	{
		FVector PlayerLocation = OwningPlayerPawn->GetActorLocation();
		FRotator PlayerRotator = OwningPlayerPawn->GetActorRotation();
		MapSize = MiniMap->GetDesiredSize();
		ratio = MapSize.X / (MapDimension*0.5f);

		for (auto item : MiniMapInfo)
		{
			if (item.IsStatic == false)
			{
				FWidgetTransform NewTransform;

				FVector2D OutDir;
				float OutLength;
				if (item.Item && item.Item->IsA<AActor>())
				{
					FVector OtherPlayerLocation = item.Item->GetActorLocation();
					FRotator OtherPlayerRotation = item.Item->GetActorRotation();

					FVector RelativeLocation = OtherPlayerLocation - PlayerLocation;
					NewTransform.Translation = FVector2D(RelativeLocation.Y, -RelativeLocation.X)*ratio;

					NewTransform.Translation.ToDirectionAndLength(OutDir, OutLength);
					OutLength = FMath::Clamp(OutLength, 0.0f, 200.0f);

					NewTransform.Translation = OutDir * OutLength;

					MapIcon->SetRenderTransformAngle(PlayerRotator.Yaw);
					item.Widget->SetRenderTransform(NewTransform);
					item.Widget->SetRenderTransformAngle(OtherPlayerRotation.Yaw);

				}
				
				//UE_LOG(LogTemp, Warning, TEXT("ViewPortSize		%f  ...  %f  ...   %f"),ratio,MapSize.X,MapDimension);
			}
		}
	}
	
	
}

void UMiniMapUserWidget::AddNewItem(FMiniMapInfo& Item)
{
	if (NewItemClass)
	{
		UIconWidget *UserWidget = Cast<UIconWidget>(CreateWidget(GetWorld(), NewItemClass));
		if (UserWidget)
		{
			FSlateBrush NewIconBrush;
			NewIconBrush.SetResourceObject ( Item.ItemIcon );
			
			if (Item.TeamAttitude == ETeamAttitude::Friendly)
			{
				NewIconBrush.TintColor = FSlateColor(FColor::Green);
				//UE_LOG(LogTemp, Warning, TEXT(" GREEN  "));				
			}
			else if (Item.TeamAttitude == ETeamAttitude::Hostile)
			{
				NewIconBrush.TintColor = FSlateColor(FColor::Red);
				//UE_LOG(LogTemp, Warning, TEXT(" RED  "));
			}			
			
			UserWidget->IconImage->SetBrush(NewIconBrush);	
			

			UserWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));			
			
			FAnchors Anchor(0.5f, 0.5f);
			MiniMapCanvas->AddChildToCanvas(UserWidget);

			UCanvasPanelSlot  *CanvasSlot = Cast<UCanvasPanelSlot>(UserWidget->Slot);

			if (CanvasSlot)
			{
				CanvasSlot->SetAnchors(Anchor);
				FVector2D Center(0.5f, 0.5f);
				CanvasSlot->SetAlignment(Center);
			
			}		

			Item.Widget = UserWidget;			
		}
	}

	MiniMapInfo.Add(Item);
	
}
