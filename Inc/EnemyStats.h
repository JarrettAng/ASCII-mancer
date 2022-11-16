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
	//Maybe have an enum for enemy type
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
//Set how many types are there
//int enemy_TotalType;
void CreateEnemy(int cost, int speed, int health,int damage, const char* sprite,ZombieType type,CP_Color color);
void InitEnemyPool(void);		//Fills the enemyArray with predefined enemies
void MoveEnemy(EnemyInfo* enemy);
void OnDeath(void);

void DrawEnemy(EnemyInfo* enemy);

//Helper functions
int GetEnemyCount(void);
EnemyInfo* GetEnemyPrefab(int index);
EnemyInfo* GetRandomEnemyPrefab(void);
void ClearEnemyPool(void);
void SendDamage(int x, int y,int damage);