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

/** Structure that defines the number of enemies per wave*/
USTRUCT(BlueprintType)
struct FEnemySpawnNumber : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEnemySpawnNumber()
		: LightEnemies(0)
		, MediumEnemies(0)
		, HeavyEnemies(0)
		, TotalEnemies(0)
		, MinPoints(0)
	{}

	/** The 'Name' column is the same as the Wave Level */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnNumber)
		int32 LightEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnNumber)
		int32 MediumEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnNumber)
		int32 HeavyEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnNumber)
		int32 TotalEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnNumber)
		int32 MinPoints;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Damage;

};

UCLASS()
class PROJECTANGELIS_API AWeapon : public AActor
{

	GENERATED_BODY()	

public:

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void AttachToPlayer();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void DetachFromPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float TimeBetweenShots;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 CurrentClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UAnimationAsset* fireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UAnimationAsset* reloadMontage;

	AWeapon(const class FObjectInitializer& PCIP);	

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Instant_Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	USkeletalMeshComponent* WeaponMesh;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;
		
	UFUNCTION(BlueprintCallable, Category = Weapon)
	virtual void ProjectileFire();	

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARocket> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void OnEquip();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void OnUnEquip();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetOwningPawn(AAngelisCharacter *NewOwner);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void ReloadAmmo();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	AAngelisCharacter *MyPawn;

};


