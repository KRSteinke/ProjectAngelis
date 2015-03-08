// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "IntegratedCharacterNoAI.h"
#include "UnrealNetwork.h"

AIntegratedCharacterNoAI::AIntegratedCharacterNoAI(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	DefaultBaseEyeHeight = 71.f;
}

void AIntegratedCharacterNoAI::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, UTReplicatedMovement, COND_SimulatedOrPhysics);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, GoodMoveAckTime, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, Health, COND_None); // would be nice to bind to teammates and spectators, but that's not an option :(

	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, InventoryList, COND_OwnerOnly);
	// replicate for cases where non-owned inventory is replicated (e.g. spectators)
	// UE4 networking doesn't cause endless replication sending unserializable values like UE3 did so this shouldn't be a big deal
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, InventoryList, COND_None);

	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, FlashCount, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, FlashLocation, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, FireMode, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, LastTakeHitInfo, COND_Custom);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, WeaponClass, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, WeaponAttachmentClass, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, bApplyWallSlide, COND_SkipOwner);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, HolsteredWeaponAttachmentClass, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, DamageScaling, COND_None);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, FireRateMultiplier, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, AmbientSound, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, CharOverlayFlags, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, WeaponOverlayFlags, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, ReplicatedBodyMaterial, COND_None);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, HeadScale, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, bUnlimitedAmmo, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, bFeigningDeath, COND_None);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, bRepDodgeRolling, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, bSpawnProtectionEligible, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, EmoteReplicationInfo, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, EmoteSpeed, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, DrivenVehicle, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, bHasHighScore, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, WalkMovementReductionPct, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, WalkMovementReductionTime, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, bInvisible, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, HatClass, COND_None);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, HatFlashCount, COND_Custom);
	//DOREPLIFETIME_CONDITION(AIntegratedCharacterNoAI, HatSpreeCount, COND_None);
}

FVector AIntegratedCharacterNoAI::GetRewindLocation(float PredictionTime)
{
	FVector TargetLocation = GetActorLocation();
	float TargetTime = GetWorld()->GetTimeSeconds() - PredictionTime;
	if (PredictionTime > 0.f)
	{
		for (int32 i = SavedPositions.Num() - 1; i >= 0; i--)
		{
			TargetLocation = SavedPositions[i].Position;
			if (SavedPositions[i].Time < TargetTime)
			{
				if (!SavedPositions[i].bTeleported && (i<SavedPositions.Num() - 1))
				{
					float Percent = (SavedPositions[i + 1].Time == SavedPositions[i].Time) ? 1.f : (TargetTime - SavedPositions[i].Time) / (SavedPositions[i + 1].Time - SavedPositions[i].Time);
					TargetLocation = SavedPositions[i].Position + Percent * (SavedPositions[i + 1].Position - SavedPositions[i].Position);
				}
				break;
			}
		}
	}
	return TargetLocation;
}

FVector AIntegratedCharacterNoAI::GetDelayedShotPosition()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	for (int32 i = SavedPositions.Num() - 1; i >= 0; i--)
	{
		if (SavedPositions[i].bShotSpawned)
		{
			return SavedPositions[i].Position;
		}
		if (WorldTime - SavedPositions[i].Time > MaxShotSynchDelay)
		{
			break;
		}
	}
	return GetActorLocation();
}

FRotator AIntegratedCharacterNoAI::GetDelayedShotRotation()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	for (int32 i = SavedPositions.Num() - 1; i >= 0; i--)
	{
		if (SavedPositions[i].bShotSpawned)
		{
			return SavedPositions[i].Rotation;
		}
		if (WorldTime - SavedPositions[i].Time > MaxShotSynchDelay)
		{
			break;
		}
	}
	return GetViewRotation();
}

bool AIntegratedCharacterNoAI::DelayedShotFound()
{
	const float WorldTime = GetWorld()->GetTimeSeconds();
	for (int32 i = SavedPositions.Num() - 1; i >= 0; i--)
	{
		if (SavedPositions[i].bShotSpawned)
		{
			return true;
		}
		if (WorldTime - SavedPositions[i].Time > MaxShotSynchDelay)
		{
			break;
		}
	}
	return false;
}

void AIntegratedCharacterNoAI::GetSimplifiedSavedPositions(TArray<FSavedPosition>& OutPositions) const
{
	OutPositions.Empty(SavedPositions.Num());
	if (SavedPositions.Num() > 0)
	{
		OutPositions.Add(SavedPositions[0]);
		for (int32 i = 1; i < SavedPositions.Num(); i++)
		{
			if (OutPositions.Last().Time < SavedPositions[i].Time)
			{
				OutPositions.Add(SavedPositions[i]);
			}
		}
	}
}

void AIntegratedCharacterNoAI::HeadScaleUpdated()
{
	// TODO
}
float AIntegratedCharacterNoAI::GetFireRateMultiplier()
{
	return FMath::Max<float>(FireRateMultiplier, 0.1f);
}


EAllowedSpecialMoveAnims AIntegratedCharacterNoAI::AllowedSpecialMoveAnims()
{
	// All emotes are full body at the moment and we're having issues with remote clients not seeing full body emotes
	/*
	if (CharacterMovement != NULL && (!CharacterMovement->IsMovingOnGround() || !CharacterMovement->GetCurrentAcceleration().IsNearlyZero()))
	{
	return EASM_UpperBodyOnly;
	}
	*/

	return EASM_Any;
}

float AIntegratedCharacterNoAI::GetRemoteViewPitch()
{
	float ClampedPitch = (RemoteViewPitch * 360.0 / 255.0);
	ClampedPitch = ClampedPitch > 90 ? ClampedPitch - 360 : ClampedPitch;
	return FMath::Clamp<float>(ClampedPitch, -90.0, 90.0);
}

void AIntegratedCharacterNoAI::BeginPlay()
{
	GetMesh()->SetOwnerNoSee(false); // compatibility with old content, we're doing this through UpdateHiddenComponents() now

	if (GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		APlayerController* PC = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (PC != NULL && PC->MyHUD != NULL)
		{
			PC->MyHUD->AddPostRenderedActor(this);
		}
	}
	if (Health == 0 && Role == ROLE_Authority)
	{
		Health = HealthMax;
	}
	CharacterCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, DefaultBaseEyeHeight), false);
	if (CharacterCameraComponent->RelativeLocation.Size2D() > 0.0f)
	{
		//UE_LOG(UT, Warning, TEXT("%s: CameraComponent shouldn't have X/Y translation!"), *GetName());
	}
	Super::BeginPlay();
}

void AIntegratedCharacterNoAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetNetMode() != NM_DedicatedServer)
	{
		BodyMI = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	}
}

void AIntegratedCharacterNoAI::SetAmbientSound(USoundBase* NewAmbientSound, bool bClear)
{
	if (bClear)
	{
		if (NewAmbientSound == AmbientSound)
		{
			AmbientSound = NULL;
		}
	}
	else
	{
		AmbientSound = NewAmbientSound;
	}
	AmbientSoundUpdated();
}

void AIntegratedCharacterNoAI::AmbientSoundUpdated()
{
	if (AmbientSound == NULL)
	{
		if (AmbientSoundComp != NULL)
		{
			AmbientSoundComp->Stop();
		}
	}
	else
	{
		if (AmbientSoundComp == NULL)
		{
			AmbientSoundComp = NewObject<UAudioComponent>(this);
			AmbientSoundComp->bAutoDestroy = false;
			AmbientSoundComp->bAutoActivate = false;
			AmbientSoundComp->AttachTo(RootComponent);
			AmbientSoundComp->RegisterComponent();
		}
		if (AmbientSoundComp->Sound != AmbientSound)
		{
			// don't attenuate/spatialize sounds made by a local viewtarget
			AmbientSoundComp->bAllowSpatialization = true;
			for(FLocalPlayerIterator It(GEngine, GetWorld()); It; ++It)
			{
				if (It->PlayerController != NULL && It->PlayerController->GetViewTarget() == this)
				{
					AmbientSoundComp->bAllowSpatialization = false;
					break;
				}
			}
			AmbientSoundComp->SetSound(AmbientSound);
		}
		if (!AmbientSoundComp->IsPlaying())
		{
			AmbientSoundComp->Play();
		}
	}
}

void AIntegratedCharacterNoAI::SetLocalAmbientSound(USoundBase* NewAmbientSound, float SoundVolume, bool bClear)
{
	if (bClear)
	{
		if ((NewAmbientSound != NULL) && (NewAmbientSound == LocalAmbientSound))
		{
			LocalAmbientSound = NULL;
			LocalAmbientSoundUpdated();
		}
	}
	else
	{
		LocalAmbientSound = NewAmbientSound;
		LocalAmbientSoundUpdated();
		if (LocalAmbientSoundComp && LocalAmbientSound)
		{
			LocalAmbientSoundComp->SetVolumeMultiplier(SoundVolume);
		}
	}
}

void AIntegratedCharacterNoAI::LocalAmbientSoundUpdated()
{
	if (LocalAmbientSound == NULL)
	{
		if (LocalAmbientSoundComp != NULL)
		{
			LocalAmbientSoundComp->Stop();
		}
	}
	else
	{
		if (LocalAmbientSoundComp == NULL)
		{
			LocalAmbientSoundComp = NewObject<UAudioComponent>(this);
			LocalAmbientSoundComp->bAutoDestroy = false;
			LocalAmbientSoundComp->bAutoActivate = false;
			//	LocalAmbientSoundComp->bAllowSpatialization = false;
			LocalAmbientSoundComp->AttachTo(RootComponent);
			LocalAmbientSoundComp->RegisterComponent();
		}
		if (LocalAmbientSoundComp->Sound != LocalAmbientSound)
		{
			LocalAmbientSoundComp->SetSound(LocalAmbientSound);
		}
		if (!LocalAmbientSoundComp->IsPlaying())
		{
			LocalAmbientSoundComp->Play();
		}
	}
}

void AIntegratedCharacterNoAI::SetStatusAmbientSound(USoundBase* NewAmbientSound, float SoundVolume, float PitchMultiplier, bool bClear)
{
	if (bClear)
	{
		if ((NewAmbientSound != NULL) && (NewAmbientSound == StatusAmbientSound))
		{
			StatusAmbientSound = NULL;
			StatusAmbientSoundUpdated();
		}
	}
	else
	{
		StatusAmbientSound = NewAmbientSound;
		StatusAmbientSoundUpdated();
		if (StatusAmbientSoundComp && StatusAmbientSound)
		{
			StatusAmbientSoundComp->SetVolumeMultiplier(SoundVolume);
			//StatusAmbientSoundComp->SetPitchMultiplier(PitchMultiplier);
		}
	}
}

void AIntegratedCharacterNoAI::StatusAmbientSoundUpdated()
{
	if (StatusAmbientSound == NULL)
	{
		if (StatusAmbientSoundComp != NULL)
		{
			StatusAmbientSoundComp->Stop();
		}
	}
	else
	{
		if (StatusAmbientSoundComp == NULL)
		{
			StatusAmbientSoundComp = NewObject<UAudioComponent>(this);
			StatusAmbientSoundComp->bAutoDestroy = false;
			StatusAmbientSoundComp->bAutoActivate = false;
			//	StatusAmbientSoundComp->bAllowSpatialization = false;
			StatusAmbientSoundComp->AttachTo(RootComponent);
			StatusAmbientSoundComp->RegisterComponent();
		}
		if (StatusAmbientSoundComp->Sound != StatusAmbientSound)
		{
			StatusAmbientSoundComp->SetSound(StatusAmbientSound);
		}
		if (!StatusAmbientSoundComp->IsPlaying())
		{
			StatusAmbientSoundComp->Play();
		}
	}
}

void AUTCharacter::SetBodyColorFlash(const UCurveLinearColor* ColorCurve, bool bRimOnly)
{
	BodyColorFlashCurve = ColorCurve;
	BodyColorFlashElapsedTime = 0.0f;
	if (BodyMI != NULL)
	{
		static FName NAME_FullBodyFlashPct(TEXT("FullBodyFlashPct"));
		BodyMI->SetScalarParameterValue(NAME_FullBodyFlashPct, bRimOnly ? 0.0f : 1.0f);
	}
}