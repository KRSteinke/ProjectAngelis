// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UsableActor.generated.h"

UCLASS()
class PROJECTANGELIS_API AUsableActor : public AStaticMeshActor
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintImplementableEvent)
		bool OnUsed(ACharacter* character);

	UFUNCTION(BlueprintImplementableEvent)
		bool StartFocusItem();

	UFUNCTION(BlueprintImplementableEvent)
		bool EndFocusItem();
	
	
};
