// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "UsableActor.h"
#include "AngelisCharacter.generated.h"


UCLASS(config = game)
class PROJECTANGELIS_API AAngelisCharacter : public ACharacter
{
	GENERATED_BODY()

	AAngelisCharacter(const class FObjectInitializer& PCIP);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void FireWeapon();

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
	TSubclassOf<class AWeapon> WeaponSpawn;

	AWeapon *CurrentWeapon;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void ProcessWeaponPickup(AWeapon *Weapon);

	

	void PrevWeapon();
	void EquipWeapon(AWeapon *Weapon);
	void GiveDefaultWeapon();

	void Die();

public:

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void NextWeapon();

	// Sets default values for this character's properties
	AAngelisCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	/*
	//Called for forwards/backwards input
	void MoveForward(float Value);

	//Called for side to side input
	void MoveRight(float Value);

	//Called via input to turn at a given rate
	//@param Rate this is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void TurnAtRate(float Rate);

	//Called via input to turn look up/down at a given rate.
	void LookUpAtRate(float Rate);
	*/
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	TArray<class AWeapon*> Inventory;

	//Base turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float BaseTurnRate;
	
	//Base Look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float BaseLookUpRate;

/*	//Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* FollowCamera;

	//Camera Boom Positioning the Camera Behind the Character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	USpringArmComponent* CameraBoom;
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* CollisionComp;

	/* Max distance to use/focus on actors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxUseDistance;

	/** Get actor derived from UsableActor currently looked at by the player */
	class AUsableActor* GetUsableInView();

	bool bHasNewFocus;
	AUsableActor* FocusedUsableActor;

	

public:

	UPROPERTY(EditDefaultsOnly, Category = Health)
	int32 Health;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	int32 MaxHealth;

	//For now, SET IT TO 3!!!
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int32 InventorySize;

	void IncreaseHealth(int32 IncreaseValue);
	void DecreaseHealth(int32 DecreaseValue);



	UPROPERTY(EditDefaultsOnly, Category = Wallet)
	int32 Wallet;

	UFUNCTION(BlueprintCallable, Category = Wallet)
	void IncreasePoints(int32 IncreaseValue);
	UFUNCTION(BlueprintCallable, Category = Wallet)
	void DecreasePoints(int32 DecreaseValue);

	UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		virtual void Use();
	virtual void Use_Implementation();
	virtual bool Use_Validate();
};
