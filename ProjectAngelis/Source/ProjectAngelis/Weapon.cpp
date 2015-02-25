// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "Weapon.h"
#include "WeaponEssentialsCharacter.h"
#include "Engine.h"

AWeapon::AWeapon(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CollisionComp = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
}

void AWeapon::Fire()
{
	if (ProjectileType == EWeaponProjectile::EBullet)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Bullet"));
		Instant_Fire();
	}
	if (ProjectileType == EWeaponProjectile::ESpread)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Spread"));
		for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
		{
			Instant_Fire();
		}
	}
	if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));
		ProjectileFire();
	}
}

void AWeapon::SetOwningPawn(AWeaponEssentialsCharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::OnEquip()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
}

void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

void AWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent *Character = MyPawn->GetMesh();
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachTo(Character, "Weapon_Socket");
	}
}

void AWeapon::DetachFromPlayer()
{
	WeaponMesh->DetachFromParent();
	WeaponMesh->SetHiddenInGame(true);
}

void AWeapon::ReloadAmmo()
{
	if (CurrentAmmo > 0)
	{
		if (CurrentAmmo < WeaponConfig.MaxClip)
		{
			CurrentClip = CurrentAmmo;
		}
		else
		{
			CurrentAmmo -= WeaponConfig.MaxClip;
			CurrentClip += WeaponConfig.MaxClip;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Blue, "No Ammo");
	}
}

void AWeapon::Instant_Fire()
{
	if (CurrentClip > 0)
	{
		const int32 RandomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomSeed);
		const float CurrentSpread = WeaponConfig.WeaponSpread;
		const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
		const FVector AimDir = WeaponMesh->GetSocketRotation("MF").Vector();
		FVector StartTrace = WeaponMesh->GetSocketLocation("MF");
		const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
		const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
		const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

		ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
		CurrentClip -= WeaponConfig.ShotCost;
	}
	else
	{
		ReloadAmmo();
	}
}

FHitResult AWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Black, true, 10000, 10.f);

	AEnemy *Enemy = Cast<AEnemy>(Impact.GetActor());
	if (Enemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT AN ENEMY!!");
		Enemy->Destroy();
	}
	AWeaponEssentialsCharacter *Character = Cast<AWeaponEssentialsCharacter>(Impact.GetActor());
	if (Character)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A CHARACTER!!");
		Character->DecreaseHealth(10);
	}
}

void AWeapon::ProjectileFire()
{

}
