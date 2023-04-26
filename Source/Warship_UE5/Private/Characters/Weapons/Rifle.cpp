// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Weapons/Rifle.h"

#include "Components/SkeletalMeshComponent.h"
#include "Characters/SlashCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h" //ForSound, ForSpawnEmitter
#include "Engine/SkeletalMeshSocket.h"  //ForTransfor

ARifle::ARifle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	RifleSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RifleSKMesh->SetupAttachment(GetRootComponent());

;
	
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
	
}


void ARifle::Equip(USceneComponent* InParent, FName InSocketName)
{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

		{
			RifleSKMesh->AttachToComponent(InParent, TransformRules, InSocketName);
		}
		
	

}

void ARifle::FireRifle()
{

	const USkeletalMeshSocket* BarrelSocket = RifleSKMesh->GetSocketByName("BarrelSocket");

	



	AActor* CurrentOwner = GetOwner();
	SlashCharacter = Cast<ASlashCharacter>(CurrentOwner);


	if (SlashCharacter)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		FVector ShotDirection = EyeRotation.Vector();
		SlashCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 6000.0f);

	
		FCollisionQueryParams QueryParams; //Para anadir actores como parametros para que se ignoren
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true; //Por defecto es falso, nos da una informacion adicional de la colision
		FHitResult HitResult;
		bool bhit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);

		FVector BeamEndPoint=TraceEnd ;

		if (bhit)
		{
		/*	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0.0f, 1.0f);*/
			DrawDebugPoint(GetWorld(), HitResult.Location, 5.f, FColor::Red, false, 2.f); 

			BeamEndPoint = HitResult.ImpactPoint;

			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location);
			}

		}
		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(RifleSKMesh);
			if (MuzzleFlash)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
			}

			if (BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);

				if (Beam)
				{
					Beam->SetVectorParameter(TraceParamName, BeamEndPoint);
				}

			}
		}
	}

}



void ARifle::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	//if (SlashCharacter)
	//{
	//	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	//	WeaponMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RigthHandtRifleSocket"));
	//}
}

void ARifle::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}