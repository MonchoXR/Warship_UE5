// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UCharacterMovementComponent;
class AWeapon;
class UAnimMontage;
class USoundCue;
class ARifle;
UCLASS()
class WARSHIP_UE5_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();

private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;


	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UCharacterMovementComponent* CharacterMovementComponent;
	UPROPERTY(VisibleInstanceOnly)

	AWeapon* OverlappingWeapon;
	
	ARifle* OverlappingRifle;
	

	//Se ha creado ENUM en otro file para que sus variables puedan ser tomadas por las animaciones
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	//Este UPROPERTY nos permite acceder a Bluerprint con la variable estando privada
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;
/**
* Animation montages
*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontageRifle;

public:
	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	
	virtual FVector GetPawnViewLocation() const override;
	/**
	* Callbacks for input
	*/
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	void Attack();
	

	/**
	* Play montage functions
	*/
	void PlayAttackMontage();
	void PlayAttackRifleMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void ActiveRifleAnimNotif();
	bool CanAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
