// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "MechPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MechAttributeSet.h"
#include "MechGame.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "Components/WidgetComponent.h"
#include "Ability/GameplayAbilityBase.h"
#include "HealthBarUserWidget.h"
#include "Net/UnrealNetwork.h"


////#define print(x)  GEngine->AddOnScreenDebugMessage(-1,8.0f,FColor::Green,x)
//#define prints(x)  GEngine->AddOnScreenDebugMessage(-1,8.0f,FColor::Green,FString::Printf(TEXT("%s"),x))
//#define printn(x)  GEngine->AddOnScreenDebugMessage(-1,8.0f,FColor::Green,FString::Printf(TEXT("%i"),x))

//////////////////////////////////////////////////////////////////////////
// AMechGameCharacter




AMechGameCharacter::AMechGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HealthComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthComponent"));
	HealthComponent->SetupAttachment(RootComponent);
	HealthComponent->InitWidget();
	HealthComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthComponent->SetDrawAtDesiredSize(true);
	HealthComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthComponent->UpdateWidget();
	HealthComponent->SetIsReplicated(true);

	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	AbilitySystemComponent->SetIsReplicated(true);
	
	MechAttributeSet = CreateDefaultSubobject<UMechAttributeSet>(TEXT("AttributeSet"));

	
	IsBound = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	GetCapsuleComponent()->OnClicked.AddUniqueDynamic(this, &AMechGameCharacter::Clicked);

	AbilitySystemComponent->SetIsReplicated(true);
}

void AMechGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	
	
	UAbilitySystemGlobals::Get().InitGlobalData();
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (auto& Ability : AbilityMap)
		{
			FGameplayAbilitySpecHandle AbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(*Ability.Value, 1, (int32)Ability.Key));
			AbilitySystemComponent->ServerSetInputPressed(AbilityHandle);
		}

		for (auto& PassiveAbility : PassiveAbilityList)
		{
			if (PassiveAbility)
			{
			
				FGameplayAbilitySpecHandle PassiveAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(*PassiveAbility, 1));
			}
			
			
		}
		
	}
	

	MyWidgetInstance = Cast<UHealthBarUserWidget>(HealthComponent->GetUserWidgetObject());	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MechAttributeSet->GetHealthAttribute()).AddUObject(MyWidgetInstance, &UHealthBarUserWidget::UpdateHealth);
	InitWidgetData();


	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitHealthEffect, 1, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	AbilitySystemComponent->ForceReplication();
	
}

void AMechGameCharacter::InitWidgetData()
{
	FOnAttributeChangeData HealthInitData;
	HealthInitData.NewValue = MechAttributeSet->GetHealth();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MechAttributeSet->GetHealthAttribute()).Broadcast(HealthInitData);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMechGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMechGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMechGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMechGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMechGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMechGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMechGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMechGameCharacter::OnResetVR);
	
	if (/*HasAuthority() && */!IsBound)
	{
		FGameplayAbilityInputBinds  bindinfo(FString("ConfirmInput"), FString("CancelInput"), "EMechAbilityInput", static_cast<int32>(EMechAbilityInput::ConfirmInput), static_cast<int32>(EMechAbilityInput::CancelInput));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, bindinfo);
		IsBound = true;
	}
}


void AMechGameCharacter::Clicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	AMechGameCharacter *MyPawn = Cast<AMechGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AMechPlayerController *MyPlayerController = Cast<AMechPlayerController>(MyPawn->GetController());
	MyPlayerController->Server_Clicked(this);
	MyPlayerController->bShowMouseCursor = false;
	MyPlayerController->bEnableClickEvents = false;

	if (GetLocalRole() == ROLE_Authority)
	{
		GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
	}
	

}

UAbilitySystemComponent * AMechGameCharacter::GetAbilitySystemComponent() const
{


	return AbilitySystemComponent;
}


void AMechGameCharacter::PossessedBy(AController * Newcontroller)
{
	Super::PossessedBy(Newcontroller);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	SetOwner(Newcontroller);


	
}

void AMechGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMechGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMechGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMechGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMechGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMechGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		
	}
}

void AMechGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void AMechGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMechGameCharacter, MyWidgetInstance);
}