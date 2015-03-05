// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "IntegratedCharacterNoAI.h"

AIntegratedCharacterNoAI::AIntegratedCharacterNoAI(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	DefaultBaseEyeHeight = 71.f;
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

void AIntegratedCharacterNoAI::UnPossessed()
{
	StopFiring();

	Super::UnPossessed();
}

void AIntegratedCharacterNoAI::StopFiring()
{
	for (int32 i = 0; i < PendingFire.Num(); i++)
	{
		if (PendingFire[i])
		{
			StopFire(i);
		}
	}
}

void AIntegratedCharacterNoAI::StopFire(uint8 FireModeNum)
{
	/*if (DrivenVehicle && !DrivenVehicle->IsLocallyControlled())
	{
	UE_LOG(LogUTCharacter, Warning, TEXT("StopFire() can only be called on the owning client"));
	}
	else if (!DrivenVehicle && !IsLocallyControlled())
	{
	UE_LOG(LogUTCharacter, Warning, TEXT("StopFire() can only be called on the owning client"));
	}*/
	//else
	if (Weapon != NULL)
	{
		Weapon->StopFire(FireModeNum);
	}
	else
	{
		SetPendingFire(FireModeNum, false);
	}
}

