/*!
@file	  Enemy.h
@author	  Muhammad Farhan Bin Ahmad (b.muhammadfarhan@digipen.edu)
@date     21/11/2022
@brief    1.Creation and declaration of enemy types
		  2.Keep track of enemy movement and status
		  3.Render enemy in the grid
________________________________________________________________________________________________________*/

#pragma once

#define ENEMYPOOL 10


typedef enum ZombieType{
    ZOMBIE,
    LEAPER,
    TANK,
    BREAKER,
    GRAVE,
	WALL,
	ZOMBIE_TYPE_LENGTH
}ZombieType;
typedef struct EnemyInfo
{
	//Position 
	int x;
	int y;
	//Cost to spawn
	int Cost;
	//Movement of tiles per turn
	int MovementSpeed;
	int damage;
	//Health
	int Health;
	int MaxHealth;
	//For the time being, it's the "sprite"
	char* CharSprite;
	CP_Color Color;
	ZombieType type;
	BOOL moveCooldown;	//IUsed for grave spawn cooldowns
	//Helps check if the enemy needs to be rendered or not
	BOOL is_Alive;

}EnemyInfo;

void CreateEnemy(int cost, int speed, int health,int damage, const char* sprite,ZombieType type,CP_Color color);
void InitEnemyPool(void);
void MoveEnemy(EnemyInfo* enemy);
void DrawEnemy(EnemyInfo* enemy);

//Helper functions
int GetEnemyCount(void);
EnemyInfo* GetEnemyPrefab(int index);
EnemyInfo* GetRandomEnemyPrefab(void);
void SendDamage(int x, int y,int damage);