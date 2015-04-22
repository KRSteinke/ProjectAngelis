
#include "FWeaponStats.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Category of GamePlay Object */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		int32 MaxAmmo;

	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		int32 AmmoPerClip;

	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		int32 InitialClip;

	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	float TimeBetweenShots;
		
	/** failsafe reload duration if weapon doesn't have any animation for it */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	float NoAnimReloadDuration;
		
	FWaveEnemyAISpawnData(){
		MaxAmmo = 0;
		AmmoPerClip = 0;
		InitialClip = 0;
		TimeBetweenShots = 0.0;
		NoAnimReloadDuration = 0.0;
	}
};