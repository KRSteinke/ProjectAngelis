
#include "FWaveEnemyAISpawnData.generated.h"

USTRUCT(BlueprintType)
struct FWaveEnemyAISpawnData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Category of GamePlay Object */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy_AI_Wave_Spawn_Data")
		int32 Num_Light_Enemies;

	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy_AI_Wave_Spawn_Data")
		int32 Num_Medium_Enemies;

	/** Scriptable Use Code */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy_AI_Wave_Spawn_Data")
		int32 Num_Large_Enemies;

	/** S */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy_AI_Wave_Spawn_Data")
		int32 Total_Num_Enemies;
		
	FWaveEnemyAISpawnData(){
		Num_Light_Enemies = 0;
		Num_Medium_Enemies = 0;
		Num_Large_Enemies = 0;
		Total_Num_Enemies = 0;
	}
};