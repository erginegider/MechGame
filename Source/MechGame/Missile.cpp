// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameplayEffect.h"
#include "MechGameCharacter.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundAttenuation.h"

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovementComponent->InitialSpeed = 7000.0f;

	CollissionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ColissionCapsule"));
	CollissionCapsule->SetupAttachment(RootComponent);
	CollissionCapsule->SetRelativeRotation(FRotator(0.0f,90.0f,0.0f));


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	DamageToApply = 1.0f;

	CollissionCapsule->IgnoreActorWhenMoving(this->GetOwner(), true);
	
	CollissionCapsule->OnComponentBeginOverlap.AddDynamic(this,&AMissile::OnCapsuleBeginOverlap);
	
	CollissionCapsule->SetMassOverrideInKg(FName::FName(), 0.0f, true);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	
}




void AMissile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMechGameCharacter *OwnerCharacter = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));	
	if (OwnerCharacter == nullptr)
		return;

	FTransform SpawnTransform;
	SpawnTransform.SetScale3D(FVector(5.0f, 5.0f, 5.0f));
	
	if (OtherActor && OtherActor != this && OtherComp)
	{

			AMechGameCharacter *OtherCharacter = Cast<AMechGameCharacter>(OtherActor);

			if (OtherCharacter && OtherCharacter!=OwnerCharacter) 
			{			
				
					UAbilitySystemComponent *OtherAbilitySystemComponent = OtherCharacter->GetAbilitySystemComponent();
					if (OtherAbilitySystemComponent)
					{
						if (DamageEffect)
						{
							Server_ApplyHit(OtherAbilitySystemComponent, OtherActor);
							//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("I am : %s      Collided with : %s"), *this->GetName(), *OtherActor->GetName()));
						}
					}
				
			}
			else if(this->GetClass()!=OtherActor->GetClass() &&  OtherCharacter!=OwnerCharacter)
			{

				
				SpawnTransform.SetTranslation(GetActorLocation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, SpawnTransform.GetLocation(), FRotator(0.0, 0.0, 0.0), 5.0f,1.0f,0.0f,Attenuation);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, SpawnTransform);
				this->Destroy(); return;
			}
	
	}
	
	
}

void AMissile::Server_ApplyHit_Implementation(UAbilitySystemComponent * OtherAbilitySystemComponent, AActor * OtherActor)
{
	FGameplayEffectContextHandle Context = OtherAbilitySystemComponent->MakeEffectContext();
	FVector Location = GetActorLocation();

	Context.AddOrigin(Location);

	TArray<TWeakObjectPtr<AActor>> ActorList;
	ActorList.Add(OtherActor);
	Context.AddActors(ActorList);
	Context.AddSourceObject(OtherActor);
	FGameplayEffectSpecHandle DamageEffectSpecHandle = OtherAbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.0, Context);

	OtherAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	PlayFxEffects(FTransform(Location));
	CollissionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
	this->Destroy(); return;
}

bool AMissile::Server_ApplyHit_Validate(UAbilitySystemComponent * OtherAbilitySystemComponent, AActor * OtherActor)
{
	return true;
}


void AMissile::PlayFxEffects_Implementation(const FTransform & Location)
{
	FTransform Transform(Location);
	Transform.SetScale3D(FVector(5.0f, 5.0f, 5.0f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, Transform);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation(), FRotator(0.0, 0.0, 0.0), 5.0f, 1.0f, 0.0f, Attenuation);

}





//// Called every frame
//void AMissile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

