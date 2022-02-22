// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AdminiFightPrototypeCharacter.generated.h"

UCLASS(config=Game,Blueprintable)
class AAdminiFightPrototypeCharacter : public ACharacter
{
	GENERATED_BODY()

	

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	UFUNCTION(BlueprintCallable, Category = "AdminiFightPrototypeCharacter")
		virtual void CalculateHealth(float delta);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AdminiFightPrototypeCharacter")
		float Health = 100;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AdminiFightPrototypeCharacter")
		bool isDead = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AdminiFightPrototypeCharacter")
		float stamina = 100;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AdminiFightPrototypeCharacter")
		float energy = 100;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AdminiFightPrototypeCharacter")
		bool lockedOn = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AdminiFightPrototypeCharacter")
		bool jumping = false;
	virtual void CalculateDead();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	AAdminiFightPrototypeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AdminiFightPrototypeCharacter")
	bool isBlocking = false;*/
protected:	
	void Punch();
	void Kick();
	void Dash();
	void CJump();
	void CStopJumping();
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
};

