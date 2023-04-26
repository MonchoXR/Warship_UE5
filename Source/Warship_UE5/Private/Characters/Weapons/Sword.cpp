// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Weapons/Sword.h"
#include "Characters/SlashCharacter.h"
#include "Components/SphereComponent.h"

//
//void ASword::Equip(USceneComponent* InParent, FName InSocketName)
//{
//	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
//	WeaponMesh->AttachToComponent(InParent, TransformRules, InSocketName);
//	
//}
ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(GetRootComponent());


}
void ASword::Equip(USceneComponent* InParent, FName InSocketName)
{
	Super::Equip( InParent,  InSocketName);
	
}


void ASword::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	//if (SlashCharacter)
	//{
	//	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	//	WeaponMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));
	//}
}

void ASword::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}