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


#define ZOMBIE 1,1,1,1,"Z",ZOMBIE
#define LEAPER 5,2,1,1,"L",LEAPER
#define TANK 10,1,3,1,"T",TANK
#define WALLBREAKER 15,1,2,2,"B",BREAKER
#define GRAVE 18,0,1,0,"G",GRAVE
#define WALL 0,0,2,0,"|x|",WALL
//Gonna use their char sprite value to indicate type of enemy
//Not gonna use externs for this case because it gets messy
EnemyInfo Enemy[ENEMYPOOL];
int enemyPoolIndex = 0;
///TEST
///TEST
//Initialises the enemy pool. Edit this to add more enemy types
void InitEnemyPool(){
	enemyPoolIndex = 0;
	//Manual way of adding enemies
	CreateEnemy(WALL,MENU_GRAY);
	CreateEnemy(ZOMBIE,MENU_RED);
	CreateEnemy(LEAPER,MENU_RED);
	CreateEnemy(WALLBREAKER,MENU_RED);
	CreateEnemy(GRAVE,GREEN);
}

//Function that creates enemies and adds them to the enemy pool
void CreateEnemy(int cost, int speed, int health,int damage, const char* sprite,ZombieType type,CP_Color color){
	EnemyInfo newEnemy = {
		.Cost = cost,
		.Health = health,
		.damage = damage,
		.MaxHealth = health,
		.CharSprite = sprite,
		.MovementSpeed = speed,
		.Color = color,
		.type = type,
		.moveCooldown = FALSE
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
	//Need to use GetEnemyCount()-2 so you don't get graves that spawn graves
	return &Enemy[CP_Random_RangeInt(1,GetEnemyCount()-2)];
}

void MoveEnemy(EnemyInfo* enemy){
	if(enemy->MovementSpeed<=0)return;
	//If the enemy has reach last x element, it'll die and damage player
	for(short i =1; i<= enemy->MovementSpeed; ++i){
		//Check enemy in front of them
		if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)==NULL){//if no enemy or tombstone, will continue movement
			continue;
		}
		if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->is_Alive){	//if its a live enemy

			if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->y == enemy->y){	//if they are in the same row

				if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->x == (enemy->x-i)){	//get first live enemy infront

					if(GetAliveEnemyFromGrid(enemy->x-i,enemy->y)->type == WALL){	//if it's a wall
						ZombieDealDamage(enemy->x-i,enemy->y,enemy->damage);	//Zombie deals damage to zombie
						if(!GetAliveEnemyFromGrid(enemy->x-i,enemy->y)){		//Check if the wall is still there
							enemy->x = enemy->x-i;								//Moves into the wall's space if not there
							PlaySound(WALLBREAK,CP_SOUND_GROUP_SFX);
							return;
							//Play whatever damage anim here for walls
						}
						enemy->x = enemy->x-i+1;								//stop right behind it
					}
					else{ //If not behind a wall
						enemy->x = enemy->x-i+1;								//stop right behind it
					}
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

	RenderEnemyDisplay(GridXToPosX(enemy->x), GridYToPosY(enemy->y), enemy->Color, enemy->Health,enemy->MaxHealth, enemy->damage);
	RenderEnemyMovement(GridXToPosX(enemy->x), GridYToPosY(enemy->y), enemy->Color, enemy->MovementSpeed);

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