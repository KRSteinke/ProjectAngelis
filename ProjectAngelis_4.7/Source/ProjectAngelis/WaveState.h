// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Wave.h"
#include "GameFramework/GameState.h"
#include "WaveState.generated.h"

/**
 * 
 */
UCLASS(config=game)
class PROJECTANGELIS_API AWaveState : public AGameState
{
	GENERATED_BODY()
	
	AWaveState(const FObjectInitializer& ObjectInitializer);

public:
	int8 CurrentWave;
	Wave *GameWaves;
	int8 MaxWaves = 50;
	void HelloWorld();
	void InitializeGame(int8 MaxWaves);
	virtual void BeginPlay();
	void IncrementWave();
};
