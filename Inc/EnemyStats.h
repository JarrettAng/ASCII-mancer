#pragma once

#define ENEMYPOOL 10

typedef struct EnemyInfo
{
	//Maybe have an enum for enemy type
	//Position 
	int x;
	int y;
	//Cost to spawn
	int Cost;
	//Movement of tiles per turn
	int MovementSpeed;
	//Health
	int Health;
	//For the time being, it's the "sprite"
	char* CharSprite;
	//in the future, width height for size

	//Helps check if the enemy needs to be rendered or not
	BOOL is_Alive;

}EnemyInfo;


//Set how many types are there
//int enemy_TotalType;
void CreateEnemy(int cost,int speed,int health,const char* sprite);
void InitEnemyPool(void);		//Fills the enemyArray with predefined enemies
void SpawnEnemy(EnemyInfo* enemy, int x, int y);
void MoveEnemy(EnemyInfo* enemy);
void OnDeath(void);

void DrawEnemy(EnemyInfo* enemy);

//Helper functions
int GetEnemyCount();
EnemyInfo* GetEnemyPrefab(int index);