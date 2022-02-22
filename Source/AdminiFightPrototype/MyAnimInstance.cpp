// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"

void UMyAnimInstance::UpdateMovementSpeed()
{
	//Get the pawn and if it's valid, update the movement speed to update
	//the movement animation
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		MovementSpeed = Pawn->GetVelocity().Size();
	}
}

void UMyAnimInstance::TargetAttack(int attackType)
{
	GLog->Log("//TODO attack moves");
	if (TargetAttackMontage)
	{
		FName CurrentSection = Montage_GetCurrentSection(TargetAttackMontage);

		//Determine which section is currently playing and jump to the next section
		//is possible
		if (CurrentSection.IsNone())
		{
			Montage_Play(TargetAttackMontage);
			if (attackType == 1)
				Montage_JumpToSection(FName("FirstAttackK"), TargetAttackMontage);
		}
		else if (CurrentSection.IsEqual("FirstAttackF") && bAcceptsSecondAttackInputF)
		{
			if (attackType == 0)
				Montage_JumpToSection(FName("SecondAttackF"), TargetAttackMontage);
		}
		else if (CurrentSection.IsEqual("SecondAttackF") && bAcceptsThirdAttackInputF)
		{
			if (attackType == 0)
				Montage_JumpToSection(FName("ThirdAttackF"), TargetAttackMontage);
		}
		else if (CurrentSection.IsEqual("ThirdAttackF") && bAcceptsFirstAttackInputK)
		{
			if (attackType == 1)
				Montage_JumpToSection(FName("FirstAttackK"), TargetAttackMontage);
		}
		else if (CurrentSection.IsEqual("FirstAttackK") && bAcceptsSecondAttackInputK)
		{
			if (attackType == 1)
				Montage_JumpToSection(FName("SecondAttackK"), TargetAttackMontage);
		}
		else if (CurrentSection.IsEqual("SecondAttackK") && bAcceptsThirdAttackInputK)
		{
			if (attackType == 1)
				Montage_JumpToSection(FName("ThirdAttackK"), TargetAttackMontage);
		}
	}
}
void UMyAnimInstance::SweepAttack(int attackType)
{
	GLog->Log("//TODO attack moves");
	if (SweepAttackMontage)
	{
		FName CurrentSection = Montage_GetCurrentSection(SweepAttackMontage);

		//Determine which section is currently playing and jump to the next section
		//is possible
		if (CurrentSection.IsNone())
		{
			Montage_Play(SweepAttackMontage);
			if (attackType == 1)
				Montage_JumpToSection(FName("FirstAttackK"), SweepAttackMontage);
		}
		else if (CurrentSection.IsEqual("FirstAttackF") && bAcceptsSecondAttackInputF)
		{
			if (attackType == 0)
				Montage_JumpToSection(FName("SecondAttackF"), SweepAttackMontage);
		}
		else if (CurrentSection.IsEqual("SecondAttackF") && bAcceptsThirdAttackInputF)
		{
			if (attackType == 0)
				Montage_JumpToSection(FName("ThirdAttackF"), SweepAttackMontage);
		}
		else if (CurrentSection.IsEqual("ThirdAttackF") && bAcceptsFirstAttackInputK)
		{
			if (attackType == 1)
				Montage_JumpToSection(FName("FirstAttackK"), SweepAttackMontage);
		}
		else if (CurrentSection.IsEqual("FirstAttackK") && bAcceptsSecondAttackInputK)
		{
			if (attackType == 1)
				Montage_JumpToSection(FName("SecondAttackK"), SweepAttackMontage);
		}
		else if (CurrentSection.IsEqual("SecondAttackK") && bAcceptsThirdAttackInputK)
		{
			if (attackType == 1)
				Montage_JumpToSection(FName("ThirdAttackK"), SweepAttackMontage);
		}
	}
}