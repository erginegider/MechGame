// Fill out your copyright notice in the Description page of Project Settings.


#include "ReticleHUD.h"
#include "MechGame.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"


void AReticleHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Canvas)
	{
		FVector2D Center(Canvas->ClipX*0.5f, Canvas->ClipY*0.5f);
		FVector2D ReticleDrawPosition(Center.X - ReticleTexture->GetSurfaceWidth()*0.5f, Center.Y - ReticleTexture->GetSurfaceHeight()*0.5f);
		FCanvasTileItem TileItem(ReticleDrawPosition, ReticleTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
	
	

}
