#include <cprocessing.h>
#include <stdio.h>
#include "EnemyDisplay.h"
#include "EnemyStats.h"
#include "WaveSystem.h"
#include "Grid.h"
#include "ColorTable.h"
#include "Particles.h"
#include "Hearts.h"
#include "SoundManager.h"


#define ENEMY1 1,1,1,"Z"
#define ENEMY2 5,3,1,"L"
#define ENEMY3 10,1,3,"B"
#define TOMBSTONE 1,0,1,"T"//Tomdstone
#define WALL 0,0,2,"[]"
//Gonna use their char sprite value to indicate type of enemy
//Not gonna use externs for this case because it gets messy
EnemyInfo Enemy[ENEMYPOOL];
int enemyPoolIndex = 0;
///TEST
///TEST
//Initialises the enemy pool. Edit this to add more enemy types
void InitEnemyPool(){
	//Manual way of adding enemies
	CreateEnemy(WALL,GREEN);
	CreateEnemy(ENEMY1,MENU_GRAY);
	CreateEnemy(ENEMY2,MENU_RED);
	CreateEnemy(ENEMY3,MENU_RED);
	CreateEnemy(TOMBSTONE,GREEN);
}

//Function that creates enemies and adds them to the enemy pool
void CreateEnemy(int cost, int speed, int health, const char* sprite,CP_Color color){
	EnemyInfo newEnemy = {
		.Cost = cost,
		.Health = health,
		.MaxHealth = 3,
		.CharSprite = sprite,
		.MovementSpeed = speed,
		.Color = color
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
//Returns the the EnemyInfo in the stored index
EnemyInfo* GetRandomEnemyPrefab(void){
	return &Enemy[CP_Random_GetInt(1,GetEnemyCount())];
}

//void SpawnEnemy(EnemyInfo* enemy, int x, int y){
//	//Maybe check enemy type here to spawn the specific particle effect for that enemy
//
//	//Spawn anim here if needed
//	enemy->x = x;
//	enemy->y = y;
//}

void MoveEnemy(EnemyInfo* enemy){
	//If the enemy has reach last x element, it'll die and damage player
	for(short i =1; i<= enemy->MovementSpeed; ++i){
		//Check enemy in front of them
		if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)==NULL){//if no enemy or tombstone, will continue movement
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

	CP_Settings_Fill(enemy->Color);
	CP_Settings_TextSize(GetCellSize() / 3.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	char buffer[25] = { 0 };
	sprintf_s(buffer, 25, "%d", enemy->Health);
	CP_Font_DrawText(enemy->CharSprite, GridXToPosX(enemy->x), GridYToPosY(enemy->y));

	RenderEnemyDisplay(GridXToPosX(enemy->x), GridYToPosY(enemy->y), enemy->Color, enemy->Health, -1);
	// if (enemy->CharSprite == "4")
	// {
	// 	CP_Font_DrawText("T", GridXToPosX(enemy->x), GridYToPosY(enemy->y));
	// }
	// else
	// {
	// }

}
void OnDeath()
{
	//Check every time an enemy is kill. If theres a low enough enemy count, the next wave will start
	//UpdateWaveStatus();
}

void ClearEnemyPool(void){
	enemyPoolIndex =0;
	memset(Enemy,0,sizeof(EnemyInfo)*ENEMYPOOL);
}