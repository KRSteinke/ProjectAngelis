// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "RocketLauncher.h"

ARocketLauncher::ARocketLauncher(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void ARocketLauncher::ProjectileFire()
{
	Super::ProjectileFire();

	if (ProjectileClass != NULL)
	{
		FVector MFLoc = WeaponMesh->GetSocketLocation("Weapon_Socket");
		FRotator MFRot = WeaponMesh->GetSocketRotation("Weapon_Socket");
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		ARocket *const Rocket = GetWorld()->SpawnActor<ARocket>(ProjectileClass, MFLoc, MFRot, SpawnParams);
		if (Rocket)
		{
		
		}
	}
}




