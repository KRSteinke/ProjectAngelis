// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.h"
#include "Enemy.h"
#include "Weapon.generated.h"

class AAngelisCharacter;

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

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 ShotCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Priority;
};

UCLASS()
class PROJECTANGELIS_API AWeapon : public AActor
{

	GENERATED_BODY()

	void AttachToPlayer(); 
	void DetachFromPlayer();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentClip;

	AWeapon(const class FPostConstructInitializeProperties& PCIP);	

	UFUNCTION()
	void Instant_Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	TSubobjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	TSubobjectPtr<USkeletalMeshComponent> WeaponMesh;

	UFUNCTION()
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;
		
	UFUNCTION()
	virtual void ProjectileFire();	

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARocket> ProjectileClass;

	void OnEquip();
	void OnUnEquip();

	void SetOwningPawn(AAngelisCharacter *NewOwner);

	void ReloadAmmo();

protected:
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	AAngelisCharacter *MyPawn;

};


