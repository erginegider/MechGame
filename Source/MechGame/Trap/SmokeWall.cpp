// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeWall.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ASmokeWall::ASmokeWall()
{
 
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(120.0f,1500.0f,800.0f));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleCount = 15;
}

// Called when the game starts or when spawned
void ASmokeWall::BeginPlay()
{
	Super::BeginPlay();

	if (ParticleEffect)
	{
		
		for (int i = 0; i < ParticleCount; i++)
		{
			FVector Origin = BoxComponent->Bounds.Origin;
			FVector BoxExtent = BoxComponent->Bounds.BoxExtent;
			FVector Location = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxExtent);
			FTransform Transform(FRotator::ZeroRotator,Location,FVector(10.0f,10.0f,10.0f));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ParticleEffect,Transform);
		}

		
	}
	
}
