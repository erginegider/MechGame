// Fill out your copyright notice in the Description page of Project Settings.


#include "MapActorComponent.h"
#include "../MechGameCharacter.h"
#include "../MechPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MiniMapUserWidget.h"
#include "MapOuterUserWidget.h"
#include "Components/Image.h"
#include "GenericTeamAgentInterface.h"

// Sets default values for this component's properties
UMapActorComponent::UMapActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UMapActorComponent::SetUpMiniMap()
{
	AMechGameCharacter *MechGameCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AMechGameCharacter *OtherCharacter = Cast<AMechGameCharacter>(GetOwner());

	if (MechGameCharacter)
	{
		AMechPlayerController *MechPlayerController = Cast<AMechPlayerController>(MechGameCharacter->GetController());


		if (MechPlayerController)
		{

			UMapOuterUserWidget *MiniMapOuter = Cast<UMapOuterUserWidget>(MechPlayerController->MapWidget);

			if (MiniMapOuter)
			{
				MiniMapWidget = Cast<UMiniMapUserWidget>(MiniMapOuter->MiniMap);

				if (MiniMapWidget)
				{
					FMiniMapInfo MapInfo;

					MapInfo.Item = GetOwner();
					MapInfo.ItemIcon = ItemIcon;
					MapInfo.IsStatic = IsStatic;
					
					if (OtherCharacter&& MechGameCharacter != OtherCharacter)
					{
						MapInfo.TeamAttitude = MechPlayerController->GetTeamAttitudeTowards(*GetOwner());
						/*if (MapInfo.TeamAttitude == ETeamAttitude::Hostile)
						{
							UE_LOG(LogTemp, Warning, TEXT(" HOSTILE TEAM ATTITUDE TOWARDS ...:  Other Component Owning Character : %s   this PlayerController : %s    "), *OtherCharacter->GetName(), *(*MechPlayerController).GetName());
						}
						else if (MapInfo.TeamAttitude == ETeamAttitude::Friendly)
						{
							UE_LOG(LogTemp, Warning, TEXT("  FRIENDLY TEAM ATTITUDE TOWARDS ...:  Other Component Owning Character : %s   this PlayerController : %s    "), *OtherCharacter->GetName(), *(*MechPlayerController).GetName());
						}*/
						MiniMapWidget->AddNewItem(MapInfo);
					}
					
					
					if (!OtherCharacter)
					{

						MiniMapWidget->AddNewItem(MapInfo);

					}
				}

			}
		}
	}

}

// Called when the game starts
void UMapActorComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetWorldTimerManager().SetTimer(SetupMiniMapHandle, this, &UMapActorComponent::SetUpMiniMap, 0.2f, false);

	
}


// Called every frame
void UMapActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

