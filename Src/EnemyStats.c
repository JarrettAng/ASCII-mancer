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
			Enemy->CreditCost = 1;
			Enemy->Health = 1;
			Enemy->MovementSpeed = 1;
			//INSERT MINUS ENEMY CREDIT HERE
			break;
		}
		case 1://Normal
		{
			Enemy->CreditCost = 2;
			Enemy->Health = 2;
			Enemy->MovementSpeed = 1;
			//INSERT MINUS ENEMY CREDIT HERE
			break;
		}
		case 2://Normal
		{
			Enemy->CreditCost = 3;
			Enemy->Health = 3;
			Enemy->MovementSpeed = 1;
			//INSERT MINUS ENEMY CREDIT HERE
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
		if (Enemy[enemy_index].is_Alive == FALSE)
		{
			SetEnemy(&Enemy[enemy_index], enemy_Type);//Spawn random type of enemy
			break;
		}
	}
}
void KillEnemy()
{
	//Check every time an enemy is kill. If theres a low enough enemy count, the next wave will start
	UpdateWaveStatus();
}