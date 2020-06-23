// Fill out your copyright notice in the Description page of Project Settings.


#include "ReticleUserWidget.h"
#include "MechPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MechGameCharacter.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"

#include "Math/UnrealMathUtility.h"


void UReticleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AMechPlayerController *MyPlayerController = Cast<AMechPlayerController>(GetOwningPlayer());
	if (MyPlayerController)
	{

		MechGameCharacter=Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		

		/*MyPlayerController->OnHealthChanged.AddDynamic(this, &UMyUserWidget::UpdateHealthWidget);
*/
	}

}

void UReticleUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry , InDeltaTime);
	if (MechGameCharacter)
	{
		float CharacterSpeed = MechGameCharacter->GetVelocity().Size();
		float CharacterSpeedRate = FMath::Clamp(CharacterSpeed / MaxCharacterSpeed,0.0f,1.0f);
		float FinalDistance = FMath::Lerp<float>(0.0f, OpeningAmount, CharacterSpeedRate);
		if (SizeBox_0)
		{
			SizeBox_0->SetRenderTranslation(FVector2D(-FinalDistance, -FinalDistance));
		}

		if (SizeBox_1)
		{
			SizeBox_1->SetRenderTranslation(FVector2D(FinalDistance, -FinalDistance));
		}

		if (SizeBox_2)
		{
			SizeBox_2->SetRenderTranslation(FVector2D(FinalDistance, FinalDistance));
		}


		if (SizeBox_3)
		{
			SizeBox_3->SetRenderTranslation(FVector2D(-FinalDistance, FinalDistance));
		}

		if (Reticle4_1)
		{
			Reticle4_1->SetRenderTranslation(FVector2D(-FinalDistance, 0.0f));
		}

		if (Reticle4_2)
		{
			Reticle4_2->SetRenderTranslation(FVector2D(FinalDistance, 0.0f));
		}

		if (Reticle4_3)
		{
			Reticle4_3->SetRenderTranslation(FVector2D(0.0f, FinalDistance));
		}

		if (Reticle4_4)
		{
			Reticle4_4->SetRenderTranslation(FVector2D(0.0f,-FinalDistance));
		}


	}
}
