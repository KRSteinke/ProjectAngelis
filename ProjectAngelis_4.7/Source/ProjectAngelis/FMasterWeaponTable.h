
#include "FMasterWeaponTable.generated.h"

USTRUCT(BlueprintType)
struct FMasterWeaponTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")	
	FString AmmoType;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")	
	int32 MaxAmmoReserve;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	int32 MaxAmmoClip;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	int32 WaveAmmoRegen;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	int32 InitialAmmo;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	int32 BaseDamageClose;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	int32 BaseDamageFar;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	FString DamageType;
	
	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	float RateOfFire;
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	int32 BaseStorePrice;
	
	/** Scriptable Use Code */
	//UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	//WeaponModel;
	
	/** Scriptable Use Code */
	//UPROPERTY(BlueprintReadOnly, Category = "Weapon_Table")
	//ProjectileModel;
	
	
	
		
	FMasterWeaponTable(){
		MaxAmmoReserve = 0;
		MaxAmmoClip = 0;
		WaveAmmoRegen = 0;
		InitialAmmo = 0;
		BaseDamageClose = 0;
		BaseDamageFar = 0;
		DamageType = "placeholder";
		RateOfFire = 1.0;
		BaseStorePrice = 0;
		//WeaponModel = "placeholder";
		//ProjectileModel = "placeholder";
	}
};