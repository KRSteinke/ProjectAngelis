// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectAngelis.h"
#include "Enemy.h"

AEnemy::AEnemy(const class FPostConstructInitializeProperties& PCIP)
	:Super(PCIP)
{
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	RootComponent = Mesh;
}



