#include <cprocessing.h>
#include <stdio.h>
#include <time.h>

#include "EnemyStats.h"
#include "WaveSystem.h"
#include "Grid.h"
#include "ColorTable.h"


#define ENEMY1 1,1,1,"1"
#define ENEMY2 2,2,2,"2"
#define ENEMY3 3,3,3,"3"
//Not gonna use externs for this case because it gets messy
EnemyInfo Enemy[ENEMYPOOL];
int enemyPoolIndex = 0;
///TEST
///TEST
//Initialises the enemy pool. Edit this to add more enemy types
void InitEnemyPool(){
	//Manual way of adding enemies
	CreateEnemy(ENEMY1);
	CreateEnemy(ENEMY2);
	CreateEnemy(ENEMY3);
}

//Function that creates enemies and adds them to the enemy pool
void CreateEnemy(int cost, int speed, int health, const char* sprite){
	EnemyInfo newEnemy;
	newEnemy.Cost = cost;
	newEnemy.Health = health;
	newEnemy.CharSprite = sprite;
	newEnemy.MovementSpeed = speed;
	Enemy[enemyPoolIndex] = newEnemy;
	enemyPoolIndex++;
}

//Returns the total count of enemies in the enemy array
int GetEnemyCount(){
	return enemyPoolIndex;
}

//Returns the the EnemyInfo in the stored index
EnemyInfo* GetEnemyPrefab(int index){
	return &Enemy[index];
}


void SpawnEnemy(EnemyInfo* enemy, int x, int y){
	//Maybe check enemy type here to spawn the specific particle effect for that enemy

	//Spawn anim here if needed
	enemy->x = x;
	enemy->y = y;
	

}

void MoveEnemy(EnemyInfo* enemy){
	//If the enemy has reach last x element, it'll die and damage player
	if (enemy->x <= 0)
	{
		enemy->is_Alive = FALSE;//Should put this in OnDeath()
	}
	for(short i =0; i< enemy->MovementSpeed; ++i){
		if(GetEnemyFromGrid(enemy->x-i,enemy->y)==NULL) continue;
		if(GetEnemyFromGrid(enemy->x-i,enemy->y)->y == enemy->y){

			if(GetEnemyFromGrid(enemy->x-i,enemy->y)->x == (enemy->x-i)){
				enemy->x = GetEnemyFromGrid(enemy->x-i,enemy->y)->x+1;
				return;
			}
		}
	}
	enemy->x -= enemy->MovementSpeed;
}
void DrawEnemy(EnemyInfo* enemy){
	
	CP_Settings_Fill(MENU_RED);
	CP_Settings_TextSize(25.f);
	CP_Font_DrawText(enemy->CharSprite, GridXToPosX(enemy->x),GridYToPosY(enemy->y));

}
void OnDeath()
{
	//Check every time an enemy is kill. If theres a low enough enemy count, the next wave will start
	//UpdateWaveStatus();
}