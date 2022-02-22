// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADMINIFIGHTPROTOTYPE_API UMyAnimInstance : public UAnimInstance
{

	GENERATED_BODY()
protected:

	/*Holds a reference to the character's movement speed*/
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
		float MovementSpeed;

	/*The attack montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		UAnimMontage* TargetAttackMontage;
	/*The attack montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite)
		UAnimMontage* SweepAttackMontage;
	/*Updates the Movement Speed variable*/
	UFUNCTION(BlueprintCallable, Category = "AnimationUpdates")
		void UpdateMovementSpeed();

	/*True if the character can perform a second attack*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAcceptsSecondAttackInputF;

	/*Ture if the character can perform a third attack*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAcceptsThirdAttackInputF;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAcceptsFirstAttackInputK;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAcceptsSecondAttackInputK;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAcceptsThirdAttackInputK;
public:
	/*
	Determines which attack animation will be played
	0 = Punch, 1 = Kick, 2 = block, 3 = Fall Kick, 4 = Dash
	*/
	void TargetAttack(int attackType);
	void SweepAttack(int attackType);

};