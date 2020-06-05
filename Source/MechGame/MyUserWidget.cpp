// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"
#include "MechPlayerController.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AMechPlayerController *MyPlayerController = Cast<AMechPlayerController>(GetOwningPlayer());
	if (MyPlayerController)
	{
		if (ItemTitle)
		{
			FText NameText = FText::FromString(TEXT(""));
			ItemTitle->SetText(NameText);
		}

		MyPlayerController->OnHealthChanged.AddDynamic(this,&UMyUserWidget::UpdateHealthWidget);

	}
	
}

void UMyUserWidget::UpdateHealthWidget(float NewHealth)
{
	FText Health = FText::FromString(FString::SanitizeFloat(NewHealth));
	ItemTitle->SetText(Health);
}
