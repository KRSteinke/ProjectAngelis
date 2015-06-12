// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Engine.h"
#include "Rocket.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTANGELIS_API ARocket : public AActor
{
	GENERATED_UCLASS_BODY()

	ARocket(const class FPostConstructInitializeProject& PCIP);

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UFUNCTION()
	void OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovement;
	
};
