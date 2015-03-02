// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "WeaponEssentialsCharacter.h"
#include "Engine.h"
#include "Engine/Blueprint.h"



AWeaponEssentialsCharacter::AWeaponEssentialsCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBlueprint(TEXT("Blueprint'/Game/Blueprints/Weapon/Weapon.Weapon'"));

	WeaponSpawn = NULL;

	if (WeaponBlueprint.Succeeded())
	{
		WeaponSpawn = (UClass*)WeaponBlueprint.Object->GeneratedClass;
	}*/

	CurrentWeapon = NULL;

	Inventory.SetNum(3, false);

	//Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	//Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CollisionComp = PCIP.CreateDefaultSubobject<UBoxComponent>(this, "CollisionComp");
	CollisionComp->AttachTo(RootComponent);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponEssentialsCharacter::OnCollision);

	//Configure character movement
	CharacterMovement->bOrientRotationToMovement = true;//character moves in the direction of input...
	CharacterMovement->RotationRate = FRotator(0.0f, 540.f, 0.0f);// ...at this rotation rate
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->AirControl = 0.2f;

	//Create a camera boon (pulls in towards the player if there is a collision)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;//The camera follows at this distance behind the character
	CameraBoom->bUseControllerViewRotation = true;//Rotate the arm based on the controller

	//Create a follow camera
	FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);//Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUseControllerViewRotation = false; //Camera does not rotate relative to arm

	//Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	//are set in the derived blueprint asset named MyCharacter(to avoid direct content references in C++)
}
	//Input

	void AWeaponEssentialsCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
	{

		//Set up gameplay key bindings
		check(InputComponent);
		InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
		InputComponent->BindAction("Fire", IE_Pressed, this, &AWeaponEssentialsCharacter::FireWeapon);
		InputComponent->BindAction("NextWeapon", IE_Pressed, this, &AWeaponEssentialsCharacter::NextWeapon);
		InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AWeaponEssentialsCharacter::PrevWeapon);
		/*InputComponent->BindAction("Pistol", IE_Pressed, this, &AWeaponEssentialsCharacter::EquipPistol);
		InputComponent->BindAction("Shotgun", IE_Pressed, this, &AWeaponEssentialsCharacter::EquipShotgun);
		InputComponent->BindAction("RocketLauncher", IE_Pressed, this, &AWeaponEssentialsCharacter::EquipRocketLauncher);*/

		InputComponent->BindAxis("MoveForward", this, &AWeaponEssentialsCharacter::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AWeaponEssentialsCharacter::MoveRight);

		//We have 2 versions of the rotation bindings to handle different kinds of devices differently
		//"turn" handles devices that provide an absolute delta, such as a mouse.
		//"turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
		InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
		InputComponent->BindAxis("TurnRate", this, &AWeaponEssentialsCharacter::TurnAtRate);
		InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
		InputComponent->BindAxis("LookUpRate", this, &AWeaponEssentialsCharacter::LookUpAtRate);

		//handle touch devices
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AWeaponEssentialsCharacter::TouchStarted);
	}

	/*void AWeaponEssentialsCharacter::BeginPlay()
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn, SpawnParams);
		if (Spawner)
		{
Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
CurrentWeapon = Spawner;
		}
	}*/

	void AWeaponEssentialsCharacter::BeginPlay()
	{
		Health = 100;
		InventorySize = 3;
		Currency = 0;
		GiveDefaultWeapon();
	}

	void AWeaponEssentialsCharacter::GiveDefaultWeapon()
	{
		AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
		if (Spawner)
		{
			Inventory[Spawner->WeaponConfig.Priority] = Spawner;
			CurrentWeapon = Inventory[Spawner->WeaponConfig.Priority];
			CurrentWeapon->SetOwningPawn(this);
			CurrentWeapon->OnEquip();
		}
	}

	void AWeaponEssentialsCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
	{
		//jump, but only on the first touch
		if (FingerIndex == ETouchIndex::Touch1)
		{
			Jump();
		}
	}

	void AWeaponEssentialsCharacter::TurnAtRate(float Rate)
	{
		//Calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}

	void AWeaponEssentialsCharacter::LookUpAtRate(float Rate)
	{
		//calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}

	void AWeaponEssentialsCharacter::MoveForward(float Value)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			//get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}

	void AWeaponEssentialsCharacter::MoveRight(float Value)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			//find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
		}
	}

	void AWeaponEssentialsCharacter::FireWeapon()
	{
		if (CurrentWeapon != NULL)
		{
			CurrentWeapon->Fire();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "No Weapon in Hand");
		}
	}

	void AWeaponEssentialsCharacter::NextWeapon()
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 i = 1; i < InventorySize; i++)
			{
				if (Inventory[(CurrentWeapon->WeaponConfig.Priority + i) % InventorySize] != NULL)
				{
					EquipWeapon(Inventory[(CurrentWeapon->WeaponConfig.Priority + i) % InventorySize]);
					return;
				}
			}		
		}
		else
		{
			if (Inventory[0] != NULL)
				EquipWeapon(Inventory[0]);
			else if (Inventory[1] != NULL)
				EquipWeapon(Inventory[1]);
			else if (Inventory[2] != NULL)
				EquipWeapon(Inventory[2]);
			else
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "No Weapon in inventory");
		}
	}

	/*void AWeaponEssentialsCharacter::NextWeapon()
	{
		if (CurrentWeapon != NULL)
		{
			if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != 2)
			{
				if (Inventory[CurrentWeapon->WeaponConfig.Priority + 1] == NULL)
				{
					for (int32 i = CurrentWeapon->WeaponConfig.Priority + 1; i < Inventory.Num(); i++)
					{
						if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
						{
							EquipWeapon(Inventory[i]);
						}
					}
				}
				else
				{
					EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority + 1]);
				}
			}
			else
			{
				EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
			}
		}
		else
		{
			if (Inventory[0] != NULL)
				EquipWeapon(Inventory[0]);
			else if (Inventory[1] != NULL)
				EquipWeapon(Inventory[1]);
			else if (Inventory[2] != NULL)
				EquipWeapon(Inventory[2]);
			else
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "No Weapon in inventory");
		}
	}*/

	void AWeaponEssentialsCharacter::PrevWeapon()
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 i = 1; i < InventorySize; i++)
			{
				if (Inventory[((CurrentWeapon->WeaponConfig.Priority + InventorySize) - i) % InventorySize] != NULL)
				{
					EquipWeapon(Inventory[((CurrentWeapon->WeaponConfig.Priority + InventorySize) - i) % InventorySize]);
					return;
				}
			}
		}
		else
		{
			if (Inventory[0] != NULL)
				EquipWeapon(Inventory[0]);
			else if (Inventory[1] != NULL)
				EquipWeapon(Inventory[1]);
			else if (Inventory[2] != NULL)
				EquipWeapon(Inventory[2]);
			else
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "No Weapon in inventory");
		}
	}

	/*void AWeaponEssentialsCharacter::PrevWeapon()
	{
		if (CurrentWeapon != NULL)
		{
			if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != 0)
			{
				if (Inventory[CurrentWeapon->WeaponConfig.Priority - 1] == NULL)
				{
					for (int32 i = CurrentWeapon->WeaponConfig.Priority + 1; i >= 0; i--)
					{
						if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass()))
						{
							EquipWeapon(Inventory[i]);
						}
					}
				}
				else
				{
					EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority - 1]);
				}
			}
			else
			{
				EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
			}
		}
		else
		{
			if (Inventory[0] != NULL)
				EquipWeapon(Inventory[0]);
			else if (Inventory[1] != NULL)
				EquipWeapon(Inventory[1]);
			else if (Inventory[2] != NULL)
				EquipWeapon(Inventory[2]);
			else
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "No Weapon in inventory");
		}
	}*/

	void AWeaponEssentialsCharacter::EquipWeapon(AWeapon *Weapon)
	{
		if (CurrentWeapon != NULL)
		{
			CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
			CurrentWeapon->OnUnEquip();
			CurrentWeapon = Weapon;
			Weapon->SetOwningPawn(this);
			Weapon->OnEquip();
		}
		else
		{
			CurrentWeapon = Weapon;
			CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
			CurrentWeapon->SetOwningPawn(this);
			Weapon->OnEquip();
		}
	}

	void AWeaponEssentialsCharacter::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
	{

		AWeapon *Weapon = Cast<AWeapon>(OtherActor);
		if (Weapon)
		{
			ProcessWeaponPickup(Weapon);
		}

		/*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "OnCollision 1");
		APistol *Pistol = Cast<APistol>(OtherActor);
		if (Pistol)
		{
		Inventory[0] = Pistol->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I just picked up a " + Pistol->WeaponConfig.Name);
		Pistol->Destroy();
		}
		AShotgun *Shotgun = Cast<AShotgun>(OtherActor);
		if (Shotgun)
		{
		Inventory[1] = Shotgun->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I just picked up a " + Shotgun->WeaponConfig.Name);
		Shotgun->Destroy();
		}
		ARocketLauncher *RocketLauncher = Cast<ARocketLauncher>(OtherActor);
		if (RocketLauncher)
		{
		Inventory[2] = RocketLauncher->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I just picked up a " + RocketLauncher->WeaponConfig.Name);
		RocketLauncher->Destroy();
		}*/
	}

	void AWeaponEssentialsCharacter::ProcessWeaponPickup(AWeapon *Weapon)
	{
		if (Weapon != NULL)
		{
			if (Inventory[Weapon->WeaponConfig.Priority] == NULL)
			{
				AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
				if (Spawner)
				{
					Inventory[Spawner->WeaponConfig.Priority] = Spawner;
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "You just picked up a " + Inventory[Spawner->WeaponConfig.Priority]->WeaponConfig.Name);
				}
				Weapon->Destroy();
			}
			else
			{
				if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo - Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo))
				{
					Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo += Weapon->CurrentAmmo;
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Added " + Weapon->CurrentAmmo);
					Weapon->Destroy();
				}
				else{
					if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo > Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo)
					{
						Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo = Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo;
					}
				}

			}
		}
	}

	void AWeaponEssentialsCharacter::DecreaseHealth(int32 DecreaseValue){
		Health -= DecreaseValue;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "DecreaseHealth");
		if (Health == 0)
			Die();
	}

	void AWeaponEssentialsCharacter::IncreaseHealth(int32 IncreaseValue){
		Health += IncreaseValue;
	}

	void AWeaponEssentialsCharacter::Die()
	{
		this->Destroy();
		this->CurrentWeapon->Destroy();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Die");
	}

	void AWeaponEssentialsCharacter::IncreaseCurrency(int32 IncreaseValue){
		Currency += IncreaseValue;
	}

	bool AWeaponEssentialsCharacter::DecreaseCurrency(int32 DecreaseValue){
		if (Currency < DecreaseValue)
			return false;
		else
		{
			Currency -= DecreaseValue;
			return true;
		}
	}

	/*void AWeaponEssentialsCharacter::EquipPistol()
	{
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[0], SpawnParams);
		if (Spawner)
		{
			if (CurrentWeapon != NULL)
			{
				for (int32 i = 0; i < 3; i++)
				{
					if (Inventory[i] == NULL && Inventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.Name == CurrentWeapon->WeaponConfig.Name)
					{
						Inventory[i] = NULL;
						Inventory[i] = CurrentWeapon->GetClass();
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I put " + CurrentWeapon->WeaponConfig.Name + " away in slot " + FString::FromInt(i));
					}
				}
				CurrentWeapon->Destroy();
				Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
				CurrentWeapon = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My current weapon is " + CurrentWeapon->WeaponConfig.Name);
			}
			else
			{
				Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
				CurrentWeapon = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My current weapon is " + CurrentWeapon->WeaponConfig.Name);
			}
		}
	}

	void AWeaponEssentialsCharacter::EquipShotgun()
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[1], SpawnParams);
		if (Spawner)
		{
			if (CurrentWeapon != NULL)
			{
				for (int32 i = 0; i < 3; i++)
				{
					if (Inventory[i] == NULL && Inventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.Name == CurrentWeapon->WeaponConfig.Name)
					{
						Inventory[i] = NULL;
						Inventory[i] = CurrentWeapon->GetClass();
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I put " + CurrentWeapon->WeaponConfig.Name + " away in slot " + FString::FromInt(i));
					}
				}
				CurrentWeapon->Destroy();
				Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
				CurrentWeapon = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My current weapon is " + CurrentWeapon->WeaponConfig.Name);
			}
			else
			{
				Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
				CurrentWeapon = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My current weapon is " + CurrentWeapon->WeaponConfig.Name);
			}
		}
	}

	void AWeaponEssentialsCharacter::EquipRocketLauncher()
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[2], SpawnParams);
		if (Spawner)
		{
			if (CurrentWeapon != NULL)
			{
				for (int32 i = 0; i < 3; i++)
				{
					if (Inventory[i] == NULL && Inventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.Name == CurrentWeapon->WeaponConfig.Name)
					{
						Inventory[i] = NULL;
						Inventory[i] = CurrentWeapon->GetClass();
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I put " + CurrentWeapon->WeaponConfig.Name + " away in slot " + FString::FromInt(i));
					}
				}
				CurrentWeapon->Destroy();
				Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
				CurrentWeapon = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My current weapon is " + CurrentWeapon->WeaponConfig.Name);
			}
			else
			{
				Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
				CurrentWeapon = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My current weapon is " + CurrentWeapon->WeaponConfig.Name);
			}

		}
	}*/





