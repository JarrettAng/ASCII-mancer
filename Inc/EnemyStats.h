#pragma once
typedef struct EnemyInfo
{
	int CreditCost;
	int MovementSpeed;
	int Health;

}EnemyInfo;

EnemyInfo Enemy[100];
int enemy_Index;
int Type;

void SetEnemy();