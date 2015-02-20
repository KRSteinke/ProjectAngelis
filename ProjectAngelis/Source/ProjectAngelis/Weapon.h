// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

/**
*
*/



UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet		UMETA(DisplayName = "Bullet"),
		ESpread		UMETA(DisplayName = "Spread"),
		EProjectile UMETA(DisplayName = "Projectile"),//possible syntax error
	};

}

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 ShotCost;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		float WeaponSpread;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		FString Name;

};

UCLASS()
class PROJECTANGELIS_API AWeapon : public AActor
{

	GENERATED_BODY()
public:
	AWeapon(const class FPostConstructInitializeProperties& PCIP);

	

	UFUNCTION()
	void Instant_Fire();

		

	



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	TSubobjectPtr<UBoxComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	TSubobjectPtr<USkeletalMeshComponent> WeaponMesh;

	UFUNCTION()
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;



	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	

	

};


