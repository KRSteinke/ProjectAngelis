// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "WeaponEssentialsCharacter.generated.h"


/**
 * 
 */
UCLASS(config=Game)
class PROJECTANGELIS_API AWeaponEssentialsCharacter : public ACharacter
{
	GENERATED_BODY()

	AWeaponEssentialsCharacter(const class FPostConstructInitializeProperties& PCIP);

	UFUNCTION()
	void FireWeapon();

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
	TSubclassOf<class AWeapon> WeaponSpawn;

	AWeapon *CurrentWeapon;

	//UFUNCTION()
	//virtual void BeginPlay() override;

	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);	

	/*void EquipPistol();
	void EquipShotgun();
	void EquipRocketLauncher();*/
	
	void ProcessWeaponPickup(AWeapon *Weapon);

	void NextWeapon();
	void PrevWeapon();
	void EquipWeapon(AWeapon *Weapon);
	void GiveDefaultWeapon();

	UFUNCTION(BlueprintCallable, Category = Event)
	virtual void BeginPlay() override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<class AWeapon*> Inventory;

	//Base turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	//Base Look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	//Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> FollowCamera;

	//Camera Boom Positioning the Camera Behind the Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	//Called for forwards/backwards input
	void MoveForward(float Value);

	//Called for side to side input
	void MoveRight(float Value);

	//Called via input to turn at a given rate
	//@param Rate this is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void TurnAtRate(float Rate);

	//Called via input to turn look up/down at a given rate.
	void LookUpAtRate(float Rate);
	
	void SetupPlayerInputComponent(class UInputComponent* InputComponent);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);	

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	TSubobjectPtr<class UBoxComponent> CollisionComp;




	
};
