// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectAngelis.h"
#include "SquadManager.h"
#include "FWaveEnemyAISpawnData.h"
#include "Engine/DataTable.h"

USquadManager::USquadManager(const class FObjectInitializer& PCIP)
: Super(PCIP){

	/*UDataTable* dataTable;*/
	/*
	static ConstructorHelpers::FObjectFinder<UDataTable>
		GameObjectLookupDataTable_BP(TEXT("DataTable'/Game/Data/WaveEnemyAISpawnData.WaveEnemyAISpawnData'"));
	dataTable = GameObjectLookupDataTable_BP.Object;
	*/
	/*
	static const FString ContextString(TEXT("GENERAL"));
	// o-- Search using FindRow. It returns a handle to the row.
	// Access the variables like GOLookupRow->Blueprint_Class, GOLookupRow->Usecode

	FWaveEnemyAISpawnData* RowLookup = dataTable->FindRow<FWaveEnemyAISpawnData>(
		*FString::Printf(
		TEXT("%d"),
		1),
		ContextString
		);
		
	*/
		/*
	if (RowLookup) {
		GEngine->AddOnScreenDebugMessage(
			GEngine->ScreenMessages.Num() + 1,
			6.0f,
			FColor::Green,
			*FString::Printf(
			TEXT("%d"), RowLookup->Num_Light_Enemies)
			);
	}
	
	*/

}


