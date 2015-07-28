// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "WaveState.h"
#include "Engine.h"

AWaveState::AWaveState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}

void AWaveState::HelloWorld()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Hello World!");
}

void AWaveState::BeginPlay()
{
	InitializeGame(MaxWaves);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Begin Play!");
	//Start at wave 1 for simplicity
	CurrentWave = 1;
	GameWaves[CurrentWave].DebugWaveNumber();
	IncrementWave();
	GameWaves[CurrentWave].DebugWaveNumber();
}

void AWaveState::InitializeGame(int8 MaxWaves)
{
	//One extra wave intialized to account for indexing from 1 and not 0
	GameWaves = new Wave[MaxWaves+1];	
	for (int8 i = 0; i <= MaxWaves; i++)
	{
		GameWaves[i].SetWaveNumber(i);
	}
}

void AWaveState::IncrementWave()
{
	CurrentWave++;
}
