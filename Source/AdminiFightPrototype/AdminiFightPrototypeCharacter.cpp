// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AdminiFightPrototypeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyAnimInstance.h"


//////////////////////////////////////////////////////////////////////////
// AAdminiFightPrototypeCharacter

AAdminiFightPrototypeCharacter::AAdminiFightPrototypeCharacter()
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAdminiFightPrototypeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AAdminiFightPrototypeCharacter::Block);
	//PlayerInputComponent->BindAction("Block", IE_Released, this, &AAdminiFightPrototypeCharacter::StopBlocking);
	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AAdminiFightPrototypeCharacter::Punch);
	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AAdminiFightPrototypeCharacter::Kick);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAdminiFightPrototypeCharacter::CJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAdminiFightPrototypeCharacter::CStopJumping);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AAdminiFightPrototypeCharacter::Dash);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAdminiFightPrototypeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAdminiFightPrototypeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAdminiFightPrototypeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAdminiFightPrototypeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AAdminiFightPrototypeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AAdminiFightPrototypeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAdminiFightPrototypeCharacter::OnResetVR);
}


void AAdminiFightPrototypeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAdminiFightPrototypeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AAdminiFightPrototypeCharacter::CJump()
{
	jumping = true;
	Jump();
}

void AAdminiFightPrototypeCharacter::CStopJumping()
{
	Jump();
}

void AAdminiFightPrototypeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AAdminiFightPrototypeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void AAdminiFightPrototypeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAdminiFightPrototypeCharacter::MoveForward(float Value)
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

void AAdminiFightPrototypeCharacter::MoveRight(float Value)
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

void AAdminiFightPrototypeCharacter::Punch()
{
	//Get a reference to our custom anim instance and tell it to update our character's animation
	UMyAnimInstance* AnimInstanceRef = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstanceRef && !GetCharacterMovement()->IsFalling()) {
		if (stamina > 10) {
			if (lockedOn) {
				MoveForward(10); AnimInstanceRef->TargetAttack(0);
			}
			else {
				MoveForward(10); AnimInstanceRef->SweepAttack(0);
			}
		}
	}
}

void AAdminiFightPrototypeCharacter::Kick()
{
	//Get a reference to our custom anim instance and tell it to update our character's animation
	UMyAnimInstance* AnimInstanceRef = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstanceRef && !GetCharacterMovement()->IsFalling()) {
		if (stamina > 20) {
			if (lockedOn) {
				MoveForward(10); AnimInstanceRef->TargetAttack(1);
			}
			else {
				MoveForward(10); AnimInstanceRef->SweepAttack(1);
			}
		}
	}
	/*if (AnimInstanceRef && !GetCharacterMovement()->IsFalling())
	{
		
		if (AnimInstanceRef && !GetCharacterMovement()->IsFalling())
			MoveForward(10); AnimInstanceRef->Attack(1);
		if (GetCharacterMovement() ->IsFalling())
			AnimInstanceRef->Attack(3);
		else
			AnimInstanceRef->Attack(1);
	}*/
}

void AAdminiFightPrototypeCharacter::Dash() {}

void AAdminiFightPrototypeCharacter::CalculateHealth(float Delta)
{
	Health += Delta;
	CalculateDead();
}

void AAdminiFightPrototypeCharacter::CalculateDead()
{
	if (Health <= 0)
		isDead = true;
	else
		isDead = false;
}
#if WITH_EDITOR
void AAdminiFightPrototypeCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	isDead = false;
	Health = 100;

	Super::PostEditChangeProperty(PropertyChangedEvent);

	CalculateDead();
}
#endif
