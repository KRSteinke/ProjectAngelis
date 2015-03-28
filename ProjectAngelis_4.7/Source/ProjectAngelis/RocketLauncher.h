// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTANGELIS_API ARocketLauncher : public AWeapon
{

	GENERATED_BODY()

	ARocketLauncher(const class FObjectInitializer& PCIP);
	
	virtual void ProjectileFire() override;
	
	
};
