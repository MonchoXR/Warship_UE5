// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Weapons/Weapon.h"
#include "Characters/Weapons/Sword.h"
#include "Characters/Weapons/Rifle.h"
#include "Animation/AnimMontage.h"
#include "GroomComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h" //ForSound,
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Configuracion para la rotacion con respecto a la camara del jugador*/
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	/* Configuracion para la rotacion con respecto a la camara del jugador */

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SocketOffset = FVector(0.0f, 80.0f, 80.0f);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

}




// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ASlashCharacter::Attack);
}

void ASlashCharacter::MoveForward(float Value)
{
	if (ActionState == EActionState::EAS_Attacking) return;
	if (Controller && (Value != 0.f))
	{
		/*FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);*/

		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::MoveRight(float Value)
{
	if (ActionState == EActionState::EAS_Attacking ) return;
	if (Controller && (Value != 0.f))
	{
		/*FVector Right = GetActorRightVector();
		AddMovementInput(Right, Value);*/
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASlashCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

FVector ASlashCharacter::GetPawnViewLocation() const
{

	if (IsValid(ViewCamera))
	{
		return ViewCamera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void ASlashCharacter::EKeyPressed()
{
	/*OverlappingWeapon->SetOwner(this);*/
	ASword* OverlappingSword = Cast<ASword>(OverlappingWeapon);
	if (OverlappingSword)
	{
		OverlappingSword->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}

	 OverlappingRifle = Cast<ARifle>(OverlappingWeapon);
	if (OverlappingRifle)
	{
		OverlappingRifle->Equip(GetMesh(), FName("RigthHandtRifleSocket"));
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
		OverlappingRifle->SetOwner(this);


			
	}
}

void ASlashCharacter::Attack()
{
	
	if (CanAttack() && CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	
	}
	if (CanAttack() && CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);

		PlayAttackRifleMontage();
	
	
	}
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ASlashCharacter::PlayAttackRifleMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontageRifle)
	{
		float GroundSpeed = UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity);
		AnimInstance->Montage_Play(AttackMontageRifle);
		//AnimInstance->Montage_JumpToSection(FName("AttackRifleMontage"), AttackMontageRifle);
	
		if (GroundSpeed > 0)
		{
			
			AnimInstance->Montage_JumpToSection(FName("AttackRifleShootRun"), AttackMontageRifle);
		
		}
		else
		{
			AnimInstance->Montage_JumpToSection(FName("AttackRifleMontage"), AttackMontageRifle);
		}
	

	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;

}

void ASlashCharacter::ActiveRifleAnimNotif()
{
	if(OverlappingRifle) OverlappingRifle->FireRifle();
}

bool ASlashCharacter::CanAttack()
{
	return (ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped);
}