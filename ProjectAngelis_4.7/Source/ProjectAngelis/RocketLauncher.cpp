// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "RocketLauncher.h"

ARocketLauncher::ARocketLauncher(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void ARocketLauncher::ProjectileFire()
{
	Super::ProjectileFire();

	if (CurrentClip > 0)
	{
		if (ProjectileClass != NULL)
		{
			FVector MFLoc = WeaponMesh->GetSocketLocation("MF");
			FRotator MFRot = WeaponMesh->GetSocketRotation("MF");
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = Instigator;
			ARocket *const Rocket = GetWorld()->SpawnActor<ARocket>(ProjectileClass, MFLoc, MFRot, SpawnParams);
			if (Rocket)
			{

			}
			CurrentClip -= WeaponConfig.ShotCost;
		}
	}
	else
	{
		ReloadAmmo();
	}
}




