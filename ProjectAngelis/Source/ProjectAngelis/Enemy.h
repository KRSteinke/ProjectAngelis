// Fill out your copyright notice in the Description page of Project Settings.

 #pragma once

#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTANGELIS_API AEnemy : public AActor
{
	GENERATED_UCLASS_BODY()

	//AEnemy(const class FPostConstructInitializeProperties& PCIP);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TSubobjectPtr<UStaticMeshComponent> Mesh;	
	
};
