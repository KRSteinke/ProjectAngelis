// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "Wave.h"
#include "Engine.h"

Wave::Wave()
{
	
}

Wave::~Wave()
{
}

void Wave::HelloWorld()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Wave");
}

void Wave::DebugWaveNumber()
{

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::FromInt(this->WaveNumber));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::FromInt(this->WaveNumber));
}

void Wave::SetWaveNumber(int8 WaveNumber)
{
	this->WaveNumber = WaveNumber;
}

void Wave::SetEnemies(int8 Light, int8 Medium, int8 Heavy)
{
	this->LightEnemies = Light;
	this->MediumEnemies = Medium;
	this->HeavyEnemies = Heavy;
	this->TotalEnemies = Light + Medium + Heavy;
}