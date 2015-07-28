// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class PROJECTANGELIS_API Wave
{
public:
	Wave();
	~Wave();

	
	int8 WaveNumber;
	int8 LightEnemies;
	int8 MediumEnemies;
	int8 HeavyEnemies;
	int8 TotalEnemies;

	void HelloWorld();

	void DebugWaveNumber();
	int8 GetWaveNumber();
	void SetWaveNumber(int8 WaveNumber);
	void SetEnemies(int8 Light, int8 Medium, int8 Heavy);
};
