#include <cprocessing.h>
#include <stdio.h>
#include <time.h>

#include "EnemyStats.h"
#include "WaveSystem.h"
#include "Grid.h"
#include "ColorTable.h"
#include "Particles.h"
#include "Hearts.h"
#include "SoundManager.h"


#define ENEMY1 1,1,1,"1"
#define ENEMY2 5,2,2,"2"
#define ENEMY3 10,3,3,"3"
#define ENEMY4 1,0,1,"4"//Tomdstone
//Gonna use their char sprite value to indicate type of enemy
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
	CreateEnemy(ENEMY4);
}

//Function that creates enemies and adds them to the enemy pool
void CreateEnemy(int cost, int speed, int health, const char* sprite){
	EnemyInfo newEnemy = {
		.Cost = cost,
		.Health = health,
		.MaxHealth = 3,
		.CharSprite = sprite,
		.MovementSpeed = speed
	};
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
	for(short i =1; i<= enemy->MovementSpeed; ++i){
		//Check enemy in front of them
		if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)==NULL || enemy->CharSprite != "4"){//if no enemy or tombstone, will continue movement
			continue;
		}
		if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->is_Alive){

			if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->y == enemy->y){

				if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->x == (enemy->x-i)){
					enemy->x = GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->x+1;
					return;
				}
			}
		} 
	}
	enemy->x -= enemy->MovementSpeed;
	if (enemy->x < 0 && enemy->is_Alive)
	{
		//Special despawn animation over here
		ZombieToPlayerParticle(GridXToPosX(enemy->x),GridYToPosY(enemy->y),enemy->Health);
		enemy->is_Alive = FALSE;//Should put this in OnDeath()
		LoseLife(1); // LOSE ONE LIFE FOR EACH ENEMY ENTERING THE WALL
	}
	// if((enemy->x-enemy->MovementSpeed)<0) enemy->x = 0;
	// else enemy->x-= enemy->MovementSpeed;
	PlaySound(ZOMBIEMOVE, CP_SOUND_GROUP_SFX);
}
void DrawEnemy(EnemyInfo* enemy) {

	CP_Settings_Fill(MENU_RED);
	CP_Settings_TextSize(GetCellSize() * ((float)enemy->Health / (float)enemy->MaxHealth));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	char buffer[25] = { 0 };
	sprintf_s(buffer, 25, "%d", enemy->Health);

	if (enemy->CharSprite == "4")
	{
		CP_Settings_Fill(GREEN);
		CP_Font_DrawText("T", GridXToPosX(enemy->x), GridYToPosY(enemy->y));
	}
	else
	{
		CP_Font_DrawText("Z", GridXToPosX(enemy->x), GridYToPosY(enemy->y));
	}

}
void OnDeath()
{
	//Check every time an enemy is kill. If theres a low enough enemy count, the next wave will start
	//UpdateWaveStatus();
}