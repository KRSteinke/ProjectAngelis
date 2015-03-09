// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "IntegratedCharacterNoAI.generated.h"

/**
 * 
 */

USTRUCT()
struct FRepUTMovement
{
	GENERATED_USTRUCT_BODY()

	/** @TODO FIXMESTEVE version that just replicates XY components, plus could quantize to tens easily */
	UPROPERTY()
	FVector_NetQuantize LinearVelocity;

	UPROPERTY()
	FVector_NetQuantize Location;

	/** @TODO FIXMESTEVE only need a few bits for this - maybe hide in Rotation.Roll */
	//UPROPERTY()
	//FVector_NetQuantize Acceleration;

	UPROPERTY()
	FRotator Rotation;

	FRepUTMovement()
		: LinearVelocity(ForceInit)
		, Location(ForceInit)
		//, Acceleration(ForceInit)
		, Rotation(ForceInit)
	{}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		bool bOutSuccessLocal = true;

		// update location, linear velocity
		Location.NetSerialize(Ar, Map, bOutSuccessLocal);
		bOutSuccess &= bOutSuccessLocal;
		Rotation.SerializeCompressed(Ar);
		LinearVelocity.NetSerialize(Ar, Map, bOutSuccessLocal);
		bOutSuccess &= bOutSuccessLocal;
		//Acceleration.NetSerialize(Ar, Map, bOutSuccessLocal);
		//bOutSuccess &= bOutSuccessLocal;

		return true;
	}

	bool operator==(const FRepUTMovement& Other) const
	{
		if (LinearVelocity != Other.LinearVelocity)
		{
			return false;
		}

		if (Location != Other.Location)
		{
			return false;
		}

		if (Rotation != Other.Rotation)
		{
			return false;
		}
		/*
		if (Acceleration != Other.Acceleration)
		{
		return false;
		}
		*/
		return true;
	}

	bool operator!=(const FRepUTMovement& Other) const
	{
		return !(*this == Other);
	}
};

USTRUCT(BlueprintType)
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()

	/** the amount of damage */
	UPROPERTY(BlueprintReadWrite, Category = TakeHitInfo)
	int32 Damage;
	/** the location of the hit (relative to Pawn center) */
	UPROPERTY(BlueprintReadWrite, Category = TakeHitInfo)
	FVector_NetQuantize RelHitLocation;
	/** how much momentum was imparted */
	UPROPERTY(BlueprintReadWrite, Category = TakeHitInfo)
	FVector_NetQuantize Momentum;
	/** the damage type we were hit with */
	UPROPERTY(BlueprintReadWrite, Category = TakeHitInfo)
	TSubclassOf<UDamageType> DamageType;
	/** shot direction pitch, manually compressed */
	UPROPERTY(BlueprintReadWrite, Category = TakeHitInfo)
	uint8 ShotDirPitch;
	/** shot direction yaw, manually compressed */
	UPROPERTY(BlueprintReadWrite, Category = TakeHitInfo)
	uint8 ShotDirYaw;
	/** if damage was partially or completely absorbed by inventory, the item that did so
	* used to play different effects
	*/
	//UPROPERTY(BlueprintReadWrite, Category = TakeHitInfo)
	//TSubclassOf<class AUTInventory> HitArmor;
};

/** ammo counter */
USTRUCT(BlueprintType)
struct FStoredAmmo
{
	GENERATED_USTRUCT_BODY()

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	//TSubclassOf<class AUTWeapon> Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	int32 Amount;
};

USTRUCT()
struct FEmoteRepInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint8 EmoteCount;

	UPROPERTY()
	int32 EmoteIndex;
};

USTRUCT(BlueprintType)
struct FSavedPosition
{
	GENERATED_USTRUCT_BODY()

	FSavedPosition() : Position(FVector(0.f)), Rotation(FRotator(0.f)), Velocity(FVector(0.f)), bTeleported(false), bShotSpawned(false), Time(0.f), TimeStamp(0.f) {};

	FSavedPosition(FVector InPos, FRotator InRot, FVector InVel, bool InTeleported, bool InShotSpawned, float InTime, float InTimeStamp) : Position(InPos), Rotation(InRot), Velocity(InVel), bTeleported(InTeleported), bShotSpawned(InShotSpawned), Time(InTime), TimeStamp(InTimeStamp) {};

	/** Position of player at time Time. */
	UPROPERTY()
	FVector Position;

	/** Rotation of player at time Time. */
	UPROPERTY()
	FRotator Rotation;

	/** Keep velocity also for bots to use in realistic reaction time based aiming error model. */
	UPROPERTY()
	FVector Velocity;

	/** true if teleport occurred getting to current position (so don't interpolate) */
	UPROPERTY()
	bool bTeleported;

	/** true if shot was spawned at this position */
	UPROPERTY()
	bool bShotSpawned;

	/** Current server world time when this position was updated. */
	float Time;

	/** Client timestamp associated with this position. */
	float TimeStamp;
};

UENUM(BlueprintType)
enum EAllowedSpecialMoveAnims
{
	EASM_Any,
	EASM_UpperBodyOnly,
	EASM_None,
};

USTRUCT(BlueprintType)
struct FBloodDecalInfo
{
	GENERATED_USTRUCT_BODY()

	/** material to use for the decal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DecalInfo)
	UMaterialInterface* Material;
	/** Base scale of decal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DecalInfo)
	FVector2D BaseScale;
	/** range of random scaling applied (always uniform) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DecalInfo)
	FVector2D ScaleMultRange;

	FBloodDecalInfo()
		: Material(NULL), BaseScale(32.0f, 32.0f), ScaleMultRange(0.8f, 1.2f)
	{}
};

UCLASS(config = Game, collapsecategories, hidecategories = (Clothing, Lighting, AutoExposure, LensFlares, AmbientOcclusion, DepthOfField, MotionBlur, Misc, ScreenSpaceReflections, Bloom, SceneColor, Film, AmbientCubemap, AgentPhysics, Attachment, Avoidance, PlanarMovement, AI, Replication, Input, Actor, Tags, GlobalIllumination))
class PROJECTANGELIS_API AIntegratedCharacterNoAI : public ACharacter
{
	GENERATED_BODY()

		AIntegratedCharacterNoAI(const class FPostConstructInitializeProperties& PCIP);

	//For now, do not uncomment.
	//virtual void SetBase(UPrimitiveComponent* NewBase, const FName BoneName = NAME_None, bool bNotifyActor = true) override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;

	/** Stored past positions of this player.  Used for bot aim error model, and for server side hit resolution. */
	UPROPERTY()
		TArray<FSavedPosition> SavedPositions;

	/** Maximum interval to hold saved positions for. */
	UPROPERTY()
		float MaxSavedPositionAge;

	/** Returns this character's position PredictionTime seconds ago. */
	UFUNCTION(BlueprintCallable, Category = Pawn)
		virtual FVector GetRewindLocation(float PredictionTime);

	/** Max time server will look back to found client synchronized shot position. */
	UPROPERTY(EditAnyWhere, Category = "Weapon")
		float MaxShotSynchDelay;

	/** Returns most recent position with bShotSpawned. */
	virtual FVector GetDelayedShotPosition();
	virtual FRotator GetDelayedShotRotation();

	/** Return true if there's a recent delayed shot */
	virtual bool DelayedShotFound();

	/** returns a simplified set of SavedPositions containing only the latest position for a given frame (i.e. each element has a unique Time) */
	void GetSimplifiedSavedPositions(TArray<FSavedPosition>& OutPositions) const;

	/** counters of ammo for which the pawn doesn't yet have the corresponding weapon in its inventory */
	UPROPERTY()
		TArray<FStoredAmmo> SavedAmmo;

	// use this to iterate inventory
	template<typename> friend class TInventoryIterator;

	/** apply HeadScale to mesh */
	UFUNCTION()
		virtual void HeadScaleUpdated();

	//BlueprintReadOnly should not be used on private members

public:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CharacterCameraComponent;

	/** Limit to armor stacking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		int32 MaxStackedArmor;


	UPROPERTY(BlueprintReadWrite, replicated, Category = Pawn)
		int32 Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pawn)
		int32 HealthMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pawn)
		int32 SuperHealthMax;

	/** head bone/socket for headshots */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pawn)
		FName HeadBone;
	/** head Z offset from head bone */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pawn)
		float HeadHeight;
	/** radius around head location that counts as headshot at 1.0 head scaling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Pawn)
		float HeadRadius;
	/** head scale factor (generally for use at runtime) */
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = HeadScaleUpdated, BlueprintReadOnly, Category = Pawn)
		float HeadScale;

	/** multiplier to damage caused by this Pawn */
	//Was replicated
	UPROPERTY(BlueprintReadWrite, replicated, Category = Pawn)
		float DamageScaling;

	/** accessors to FireRateMultiplier */
	UFUNCTION(BlueprintCallable, Category = Pawn)
		float GetFireRateMultiplier();

	//ReplicatedUsing = PlayTakeHitEffects, function not added yet
	UPROPERTY(BlueprintReadWrite, Category = Pawn)
		FTakeHitInfo LastTakeHitInfo;

	/** time of last SetLastTakeHitInfo() - authority only */
	UPROPERTY(BlueprintReadOnly, Category = Pawn)
		float LastTakeHitTime;

protected:

	/** multiplier to firing speed */
	//Replicated, ReplicatedUsing = FireRateChanged, function not yet added
	UPROPERTY()
		float FireRateMultiplier;

	/** indicates character is (mostly) invisible so AI only sees at short range, homing effects can't target the character, etc */
	//ReplicatedUsing = OnRep_Invisible, function not yet added
	UPROPERTY(BlueprintReadOnly, Category = Pawn)
		bool bInvisible;

public:

	inline bool IsInvisible() const
	{
		return bInvisible;
	}

	/** whether spawn protection may potentially be applied (still must meet time since spawn check in UTGameMode)
	* set to false after firing weapon or any other action that is considered offensive
	*/
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Pawn)
		bool bSpawnProtectionEligible;

	/** set temporarily during client reception of replicated properties because replicated position and switches to ragdoll may be processed out of the desired order
	* when set, OnRep_ReplicatedMovement() will be called after switching to ragdoll
	*/
	bool bDeferredReplicatedMovement;

	/** set to prevent firing (does not stop already started firing, call StopFiring() for that) */
protected:
	UPROPERTY(BlueprintReadOnly, Category = Pawn)
		bool bDisallowWeaponFiring;

public:

	inline bool IsFiringDisabled() const
	{
		return bDisallowWeaponFiring;
	}

	/** Used to replicate bIsDodgeRolling to non owning clients */
	//For now leave the replicatedusing out until the function is put in.
	//ReplicatedUsing = OnRepDodgeRolling
	UPROPERTY()
		bool bRepDodgeRolling;

	UFUNCTION(BlueprintCallable, Category = "Pawn")
		virtual EAllowedSpecialMoveAnims AllowedSpecialMoveAnims();

	UFUNCTION(BlueprintCallable, Category = Pawn)
		virtual float GetRemoteViewPitch();

	virtual APhysicsVolume* GetPawnPhysicsVolume() const override
	{
		if (IsRagdoll() && RootComponent != NULL)
		{
			// prioritize root (ragdoll) volume over MovementComponent
			return RootComponent->GetPhysicsVolume();
		}
		else
		{
			return Super::GetPawnPhysicsVolume();
		}
	}

public:
	inline bool IsRagdoll() const
	{
		return /*bFeigningDeath || bInRagdollRecovery ||*/ (RootComponent == GetMesh() && GetMesh()->IsSimulatingPhysics());
	}

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/** Get Max weapon land bob deflection at landing velocity Z of FullWeaponLandBobVelZ+WeaponLandBobThreshold */
	UPROPERTY()
		float DefaultBaseEyeHeight;

	/** runtime material instance for setting body material parameters (team color, etc) */
	UPROPERTY(BlueprintReadOnly, Category = Pawn)
		UMaterialInstanceDynamic* BodyMI;

	// firemodes with input currently being held down (pending or actually firing)
	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
		TArray<uint8> PendingFire;

protected:


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = AmbientSoundUpdated, Category = "Pawn")
		USoundBase* AmbientSound;

	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
		UAudioComponent* AmbientSoundComp;

	/** Ambient sound played only on owning client */
	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
		USoundBase* LocalAmbientSound;

	/** Volume of Ambient sound played only on owning client */
	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
		float LocalAmbientVolume;

	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
		UAudioComponent* LocalAmbientSoundComp;

	/** Status ambient sound played only on owning client */
	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
		USoundBase* StatusAmbientSound;

	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
		UAudioComponent* StatusAmbientSoundComp;

public:

	/** Ambient sound played while low in health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		USoundBase* LowHealthAmbientSound;

	/** Health threshold for low health ambient sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		int32 LowHealthAmbientThreshold;

	/** sets replicated ambient (looping) sound on this Pawn
	* only one ambient sound can be set at a time
	* pass bClear with a valid NewAmbientSound to remove only if NewAmbientSound == CurrentAmbientSound
	*/
	UFUNCTION(BlueprintCallable, Category = Audio)
		virtual void SetAmbientSound(USoundBase* NewAmbientSound, bool bClear = false);

	UFUNCTION()
		void AmbientSoundUpdated();

	/** sets local (not replicated) ambient (looping) sound on this Pawn
	* only one ambient sound can be set at a time
	* pass bClear with a valid NewAmbientSound to remove only if NewAmbientSound == CurrentAmbientSound
	*/
	UFUNCTION(BlueprintCallable, Category = Audio)
		virtual void SetLocalAmbientSound(USoundBase* NewAmbientSound, float SoundVolume = 0.f, bool bClear = false);

	UFUNCTION()
		void LocalAmbientSoundUpdated();

	/** sets local (not replicated) statud ambient (looping) sound on this Pawn
	* only one status ambient sound can be set at a time
	* pass bClear with a valid NewAmbientSound to remove only if NewAmbientSound == CurrentAmbientSound
	*/
	UFUNCTION(BlueprintCallable, Category = Audio)
	virtual void SetStatusAmbientSound(USoundBase* NewAmbientSound, float SoundVolume = 0.f, float PitchMultipier = 1.f, bool bClear = false);

	UFUNCTION()
	void StatusAmbientSoundUpdated();

protected:
	/** last time PlayFootstep() was called, for timing footsteps when animations are disabled */
	float LastFootstepTime;

	/** last FootNum for PlayFootstep(), for alternating when animations are disabled */
	uint8 LastFoot;

	/** replicated overlays, bits match entries in UTGameState's OverlayMaterials array */
	//UPROPERTY(Replicated, ReplicatedUsing = UpdateCharOverlays)
	//uint16 CharOverlayFlags;
	/** mesh with current active overlay material on it (created dynamically when needed) */
	UPROPERTY()
	USkeletalMeshComponent* OverlayMesh;

	/** replicated character material override */
	//UPROPERTY(Replicated, ReplicatedUsing = UpdateSkin)
	//UMaterialInterface* ReplicatedBodyMaterial;

	//Already declared?
	/** runtime material instance for setting body material parameters (team color, etc) */
	//UPROPERTY(BlueprintReadOnly, Category = Pawn)
	//UMaterialInstanceDynamic* BodyMI;



	/** timed full body color flash implemented via material parameter */
	UPROPERTY(BlueprintReadOnly, Category = Effects)
		const UCurveLinearColor* BodyColorFlashCurve;
	/** time elapsed in BodyColorFlashCurve */
	UPROPERTY(BlueprintReadOnly, Category = Effects)
		float BodyColorFlashElapsedTime;
	/** set timed body color flash (generally for hit effects)
	* NOT REPLICATED
	*/
	UFUNCTION(BlueprintCallable, Category = Effects)
		virtual void SetBodyColorFlash(const UCurveLinearColor* ColorCurve, bool bRimOnly);

	/** updates time and sets BodyColorFlash in the character material */
	virtual void UpdateBodyColorFlash(float DeltaTime);

	//--------------------------
	// Weapon bob and eye offset

	/** Current 1st person weapon deflection due to running bob. */
	UPROPERTY(BlueprintReadWrite, Category = WeaponBob)
		FVector CurrentWeaponBob;

	/** Max 1st person weapon bob deflection with axes based on player view */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		FVector WeaponBobMagnitude;

	/** Z deflection of first person weapon when player jumps */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		FVector WeaponJumpBob;

	/** deflection of first person weapon when player dodges */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		FVector WeaponDodgeBob;

	/** deflection of first person weapon when player slides */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		FVector WeaponSlideBob;

	/** Z deflection of first person weapon when player lands */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		FVector WeaponLandBob;

	/** Desired 1st person weapon deflection due to jumping. */
	UPROPERTY(BlueprintReadWrite, Category = WeaponBob)
		FVector DesiredJumpBob;

	/* Current jump bob (interpolating to DesiredJumpBob)*/
	UPROPERTY(BlueprintReadWrite, Category = WeaponBob)
		FVector CurrentJumpBob;

	/** Time used for weapon bob sinusoids, reset on landing. */
	UPROPERTY(BlueprintReadWrite, Category = WeaponBob)
		float BobTime;

	/** Rate of weapon bob when standing still. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float WeaponBreathingBobRate;
	/** Rate of weapon bob when running. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float WeaponRunningBobRate;

	/** How fast to interpolate to Jump/Land bob offset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float WeaponJumpBobInterpRate;

	/** How fast to decay out Land bob offset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float WeaponLandBobDecayRate;

	/** Get Max eye offset land bob deflection at landing velocity Z of FullEyeOffsetLandBobVelZ+EyeOffsetLandBobThreshold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float WeaponDirChangeDeflection;

	/** Current Eye position offset from base view position - interpolates toward TargetEyeOffset. */
	UPROPERTY(BlueprintReadWrite, Category = WeaponBob)
		FVector EyeOffset;

	/** Eyeoffset due to crouching transition (not scaled). */
	UPROPERTY(BlueprintReadWrite, Category = WeaponBob)
		FVector CrouchEyeOffset;

	/** Target Eye position offset from base view position. */
	UPROPERTY(BlueprintReadWrite, Category = WeaponBob)
		FVector TargetEyeOffset;

	/** How fast EyeOffset interpolates to TargetEyeOffset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		FVector EyeOffsetInterpRate;

	/** How fast CrouchEyeOffset interpolates to 0. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float CrouchEyeOffsetInterpRate;

	/** How fast TargetEyeOffset decays. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		FVector EyeOffsetDecayRate;

	/** Jump target view bob magnitude. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float EyeOffsetJumpBob;

	/** Jump Landing target view bob magnitude. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float EyeOffsetLandBob;

	/** Jump Landing target view bob Velocity threshold. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float EyeOffsetLandBobThreshold;

	/** Jump Landing target weapon bob Velocity threshold. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float WeaponLandBobThreshold;

	/** Get Max weapon land bob deflection at landing velocity Z of FullWeaponLandBobVelZ+WeaponLandBobThreshold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float FullWeaponLandBobVelZ;

	/** Get Max eye offset land bob deflection at landing velocity Z of FullEyeOffsetLandBobVelZ+EyeOffsetLandBobThreshold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponBob)
		float FullEyeOffsetLandBobVelZ;

	/** Get Max weapon land bob deflection at landing velocity Z of FullWeaponLandBobVelZ+WeaponLandBobThreshold */
	//Already defined
	//UPROPERTY()
	//float DefaultBaseEyeHeight;

	/** maximum amount of time Pawn stays around when dead even if visible (may be cleaned up earlier if not visible) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
		float MaxDeathLifeSpan;
	/** GetWorld()->TimeSeconds when PlayDying() was called */
	UPROPERTY(BlueprintReadOnly, Category = Death)
		float TimeOfDeath;

	/** Broadcast when the pawn has died [Server only] */
	//Unrecognized type. Presumably in one of the UT include files.
	//UPROPERTY(BlueprintAssignable)
	//FCharacterDiedSignature OnDied;

	/** Max distance for enemy player indicator */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		float PlayerIndicatorMaxDistance;

	/** Max distance for same team player indicator */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		float TeamPlayerIndicatorMaxDistance;

	/** Mark this pawn as belonging to the player with the highest score, intended for cosmetic usage only */
	UPROPERTY(ReplicatedUsing = OnRep_HasHighScore, BlueprintReadOnly, Category = Pawn)
		bool bHasHighScore;

	UFUNCTION()
		void OnRep_HasHighScore();

	UFUNCTION(BlueprintImplementableEvent)
		void HasHighScoreChanged();

	//Dependant on UTCharacterMovement
	//virtual void RecalculateBaseEyeHeight() override;

	/** Returns offset to add to first person mesh for weapon bob. */
	//Depndeant on UTCharacterMovement
	//virtual FVector GetWeaponBobOffset(float DeltaTime, AUTWeapon* MyWeapon);

	/** Returns eyeoffset transformed into current view */
	virtual FVector GetTransformedEyeOffset() const;

	//Dependant on UTPlayerController
	//virtual FVector GetPawnViewLocation() const override;

	//Dependant on UTPlayerController
	//virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;

	//Dependant on UTCharacterMovement
	//virtual void OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust) override;

	//Dependant on UTCharacterMovement
	//virtual void OnStartCrouch(float HeightAdjust, float ScaledHeightAdjust) override;

	virtual void Crouch(bool bClientSimulation = false) override;

	//Dependant on UTReplicatedEmitter
	//virtual bool TeleportTo(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest = false, bool bNoCheck = false) override;
	//UFUNCTION()
	//Dependant on UUTDmgType_BlockedTelefrag
	//virtual void OnOverlapBegin(AActor* OtherActor);

	//Dependant on IsDead()
	//UFUNCTION()
	//virtual void OnRagdollCollision(AActor* OtherActor, UPrimitiveComponent* HitComponent, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadOnly, Category = Pawn)
		bool bInRagdollRecovery;

	/** last time ragdolling corpse spawned a blood decal */
	UPROPERTY(BlueprintReadWrite, Category = Effects)
		float LastDeathDecalTime;

	/** Landing at faster than this velocity results in damage (note: use positive number) */
	UPROPERTY(Category = "Falling Damage", EditAnywhere, BlueprintReadWrite)
		float MaxSafeFallSpeed;

	//Dependant on UTWorldSettings
	//UFUNCTION(BlueprintCallable, Category = Pawn)
	//bool IsDead();


	/** returns true if any local PlayerController is viewing this Pawn */
	bool IsLocallyViewed()
	{
		for (FLocalPlayerIterator It(GEngine, GetWorld()); It; ++It)
		{
			if (It->PlayerController != NULL && It->PlayerController->GetViewTarget() == this)
			{
				return true;
			}
		}
		return false;
	}

	virtual FVector GetNavAgentLocation() const override
	{
		// push down a little to make sure we intersect with the navmesh but not so much that we get stuff on a lower level that requires a jump
		return GetActorLocation() - FVector(0.f, 0.f, FMath::Max<float>(25.0f, GetCharacterMovement()->MaxStepHeight));
	}

protected:
	/** reduces acceleration and friction while walking; generally applied temporarily by some weapons
	* (1.0 == no acceleration or friction, 0.0 == unrestricted movement)
	*/
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Movement)
		float WalkMovementReductionPct;
	/** time remaining until WalkMovementReductionPct is reset to zero */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Movement)
		float WalkMovementReductionTime;

public:
	inline float GetWalkMovementReductionPct()
	{
		return WalkMovementReductionPct;
	}
	/** sets walking movement reduction */
	//UFUNCTION(BlueprintCallable, Category = Movement);
	//Dependant on UTCharacterMovement
	//virtual void SetWalkMovementReduction(float InPct, float InDuration);

protected:
	/** destroys dead character if no longer onscreen */
	UFUNCTION()
		void DeathCleanupTimer();

	//Dependant on UTCharacterMovement
	//UFUNCTION(BlueprintNativeEvent, Category = "Pawn|Character|InternalEvents", meta = (FriendlyName = "CanDodge"))
	//bool CanDodgeInternal() const;

	/** hook to modify damage CAUSED by this Pawn - note that EventInstigator may not be equal to Controller if we're in a vehicle, etc */
	UFUNCTION(BlueprintNativeEvent)
	void ModifyDamageCaused(int32& Damage, FVector& Momentum, const FDamageEvent& DamageEvent, AActor* Victim, AController* EventInstigator, AActor* DamageCauser);

private:
	void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

public:
	/** set by path following to last MoveTarget that was successfully reached; used to avoid pathing getting confused about its current position on the navigation graph when it is on two nodes/polys simultaneously */
	//UPROPERTY()
	//FRouteCacheItemundefined
	//FRouteCacheItem LastReachedMoveTarget;
};

/*No idea what btearoff or pendingkillpending is.
inline bool AIntegratedCharacterNoAI::IsDead()
{
	return bTearOff || bPendingKillPending;
}*/