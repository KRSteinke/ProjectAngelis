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

	public:

	UFUNCTION(BlueprintCallable, Category = Weapon)
	int32 GetHealth();

	

	UFUNCTION(BlueprintCallable, Category = Weapon)
	AWeapon* GetCurrentWeapon();

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
	TSubclassOf<class AWeapon> WeaponSpawn;


	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	AWeapon *CurrentWeapon;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);



	UFUNCTION(BlueprintCallable, Category = Weapon)
	void ProcessWeaponPickup(AWeapon *Weapon);	

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void PrevWeapon();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void GiveDefaultWeapon();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void Die();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool FireWeapon();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void NextWeapon();

	// Sets default values for this character's properties
	AAngelisCharacter();

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	/*
	//Called for forwards/backwards input
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void MoveForward(float Value);

	//Called for side to side input
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void MoveRight(float Value);

	//Called via input to turn at a given rate
	//@param Rate this is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void TurnAtRate(float Rate);

	//Called via input to turn look up/down at a given rate.
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void LookUpAtRate(float Rate);
	*/
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void EquipWeapon(AWeapon *Weapon);	

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

	UPROPERTY(EditDefaultsOnly, Category = Health)
	int32 Health;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	int32 MaxHealth;

	//For now, SET IT TO 3!!!
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int32 InventorySize;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void IncreaseHealth(int32 IncreaseValue);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void DecreaseHealth(int32 DecreaseValue);



	UPROPERTY(EditDefaultsOnly, Category = Wallet)
	int32 Wallet;

	UPROPERTY(EditDefaultsOnly, Category = Wallet)
	int32 Score;


	UFUNCTION(BlueprintCallable, Category = Weapon)
	void IncreasePoints(int32 IncreaseValue);
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void DecreasePoints(int32 DecreaseValue);

	UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		virtual void Use();
	virtual void Use_Implementation();
	virtual bool Use_Validate();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool IsWeaponNull();

	UFUNCTION(BlueprintCallable, Category = Wallet)
	int32 getWallet();

	UFUNCTION(BlueprintCallable, Category = Wallet)
	int32 getScore();



};
