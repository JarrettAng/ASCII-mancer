#include <cprocessing.h>
#include <stdio.h>
#include <time.h>

#include "EnemyStats.h"
#include "WaveSystem.h"

//Call this to set enemy stats before spawning them
void SetEnemy(EnemyInfo* Enemy,int Type)
{
	switch (Type)
	{
		case 0://Normal
		{
			Enemy->Cost = 1;
			Enemy->Health = 1;
			Enemy->MovementSpeed = 1;
			break;
		}
		case 1://Normal
		{
			Enemy->Cost = 2;
			Enemy->Health = 2;
			Enemy->MovementSpeed = 1;
			break;
		}
		case 2://Normal
		{
			Enemy->Cost = 3;
			Enemy->Health = 3;
			Enemy->MovementSpeed = 1;
			break;
		}
	}
	enemy_Left++;
}
void SpawnEnemy()
{
	srand(time(0));
	int enemy_Type = (rand() % enemy_TotalType);
	//Set Check current credit here to dictate what enemy to spawn
	for (int i = 0; i < ENEMYPOOL; i++)
	{
		//Check pool to see which enemy element is free
		if (Enemy[enemyIndex].is_Alive == FALSE)
		{
			SetEnemy(&Enemy[enemyIndex], enemy_Type);//Spawn random type of enemy
			break;
		}
	}
}
void EnemyDeath()
{
	//Check every time an enemy is kill. If theres a low enough enemy count, the next wave will start
	UpdateWaveStatus();
}