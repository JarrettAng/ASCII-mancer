#pragma once

#define ENEMYPOOL 100

typedef struct EnemyInfo
{
	int CreditCost;
	int MovementSpeed;
	int Health;

	BOOL is_Alive;

}EnemyInfo;

EnemyInfo Enemy[ENEMYPOOL];

int enemy_index;
//Set how many types are there
int enemy_TotalType = 3;

void SetEnemy(EnemyInfo* Enemy, int Type);