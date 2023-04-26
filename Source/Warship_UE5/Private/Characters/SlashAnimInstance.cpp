// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (SlashCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
		IsFalling = SlashCharacterMovement->IsFalling();
		CharacterStatefromSlash = SlashCharacter->GetCharacterState();
		

		if (CharacterStatefromSlash == ECharacterState::ECS_EquippedTwoHandedWeapon)
		{
				SlashCharacterMovement->MaxWalkSpeed = 150.0f;
				// Offset Yaw for Strafing
				FRotator AimRotation = SlashCharacter->GetBaseAimRotation();
				FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(SlashCharacter->GetVelocity());
				YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
			
				
				SlashCharacterMovement->bOrientRotationToMovement = false;
				SlashCharacter->bUseControllerRotationYaw = true;
		}

		else
		{
			SlashCharacterMovement->MaxWalkSpeed = 600.0f;
		}


	
		
	}

	
}