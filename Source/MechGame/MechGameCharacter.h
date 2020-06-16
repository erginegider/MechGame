// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MechGame.h"
#include "Ability/FEffectContainer.h"
#include "GenericTeamAgentInterface.h"
#include "MechGameCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnArmorChanged, float, NewArmor);

UCLASS(config=Game)
class AMechGameCharacter : public ACharacter, public IAbilitySystemInterface , public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AMechGameCharacter();


	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Team, meta = (AllowPrivateAccess = "true"))
	FGenericTeamId TeamID;

	virtual FGenericTeamId GetGenericTeamId() const override; 
	

	//FORCEINLINE virtual  int32 GetTeamId()  { return TeamId; }



	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor & Other) const override;

	void SetGenericTeamId(const FGenericTeamId & NewTeamID) override;

	virtual void BeginPlay() override;

	void InitOnPlayerWidgetDataForBeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION()
	void Clicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UMechAttributeSet *MechAttributeSet;

	bool IsBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TMap<EAbilityKey,TSubclassOf<class UGameplayAbilityBase>> AbilityMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<TSubclassOf<class UGameplayAbilityBase>> PassiveAbilityList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GE")
	TSubclassOf<class UGameplayEffect> InitAttributeSetEffect;

	UFUNCTION()
	virtual void PossessedBy(AController *Newcontroller) override;


	UFUNCTION(NetMulticast,reliable,Category="AbilityInput")
	void ReSetupInput();


	UFUNCTION(BlueprintCallable,Category = "AbilityInput")
	void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);

	
	FOnPawnHealthChanged OnPawnHealthChanged; // fired from attributset


	FOnPawnArmorChanged OnPawnArmorChanged;  // fired from....

	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	class AController *myAIController;*/

	UFUNCTION()
	void SetupInput();

protected:

	UPROPERTY(Replicated)
	class UHealthBarUserWidget *PlayerHeadHUD;

	class TSubclassOf<class UUserWidget> HealthWidgetClass;
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(NetMulticast, BlueprintCallable, reliable)
	void Multicast_SetVisibility(bool State);
};

