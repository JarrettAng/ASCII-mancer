#include <cprocessing.h>
#include <stdio.h>
#include <time.h>

#include "EnemyStats.h"
#include "WaveSystem.h"
#include "Grid.h"
#include "ColorTable.h"

//Not gonna use externs for this case because it gets messy
EnemyInfo Enemy[ENEMYPOOL];
int enemyPoolIndex = 0;

//Initialises the enemy pool. Edit this to add more enemy types
void InitEnemyPool(){
	//Manual way of adding enemies
	CreateEnemy(1,1,1,"1");
	CreateEnemy(2,2,1,"2");
	CreateEnemy(3,3,1,"3");
}

//Function that creates enemies and adds them to the enemy pool
void CreateEnemy(int cost, int speed, int health,const char* sprite){
	EnemyInfo newEnemy;
	newEnemy.Cost = cost;
	newEnemy.Health = health;
	newEnemy.CharSprite = sprite;
	Enemy[enemyPoolIndex] = newEnemy;
	enemyPoolIndex++;
}

//Returns the total count of enemies in the enemy array
int GetEnemyCount(){
	return enemyPoolIndex;
}

//Returns the the EnemyInfo in the stored index
EnemyInfo* GetEnemy(int index){
	return &Enemy[index];
}


void SpawnEnemy(EnemyInfo* enemy, int x, int y){
	//Maybe check enemy type here to spawn the specific particle effect for that enemy

	//Spawn anim here if needed
	enemy->x = x;
	enemy->y = y;
	
	// srand(time(0));
	// int enemy_Type = (rand() % enemy_TotalType);
	// //Set Check current credit here to dictate what enemy to spawn
	// for (int i = 0; i < ENEMYPOOL; i++)
	// {
	// 	//Check pool to see which enemy element is free
	// 	if (Enemy[enemyPoolIndex].is_Alive == FALSE)
	// 	{
	// 		SetEnemy(&Enemy[enemyPoolIndex], enemy_Type);//Spawn random type of enemy
	// 		break;
	// 	}
	// }

}

void MoveEnemy(EnemyInfo* enemy){
	// if(enemy->x <=0){
	// 	//If the enemy has hit the far left of the screen, do something!
	// 	enemy->is_Alive = FALSE;
	// }
	//Moving the enemies by their movespeed * arbritrary cellsize for now.
	//NOTE!! REMEMBER TO MULTIPLY BY ACTUAL CELL SIZE!
	enemy->x+=(enemy->MovementSpeed*150);
}
void DrawEnemy(EnemyInfo* enemy){
	CP_Settings_Fill(MENU_RED);
	CP_Settings_TextSize(55.f);
	CP_Font_DrawText(enemy->CharSprite,enemy->x,enemy->y);
}
void OnDeath()
{
	//Check every time an enemy is kill. If theres a low enough enemy count, the next wave will start
	//UpdateWaveStatus();
}