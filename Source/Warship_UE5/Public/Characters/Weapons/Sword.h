// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Weapons/Weapon.h"

#include "Sword.generated.h"


class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class WARSHIP_UE5_API ASword : public AWeapon
{
	GENERATED_BODY()
public:
	ASword();

public:
	virtual void Equip(USceneComponent* InParent, FName InSocketName) override;
protected:

	//Hereda de Weapon, por tanto no se pone el UFunction y se agrega virtual al inicio
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* SwordMesh;


};

