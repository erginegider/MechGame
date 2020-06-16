// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AHomingMissile::AHomingMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("UCapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);

	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	MissileMesh->SetupAttachment(RootComponent);

	CapsuleComponent->SetCapsuleRadius(300.0f);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);
	CapsuleComponent->SetGenerateOverlapEvents(true);

	OnActorBeginOverlap.AddDynamic(this, &AHomingMissile::ActorBeginOverlap);
}


void AHomingMissile::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Purple,TEXT(" This is Pulse"));
}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

