/*!
@file	  WaveSystem.c
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@date     23/11/2022
@brief    This source file handles the AI director and spawning of enemies to the grid. 
======== WAVE SYSTEM ALGORITHM ========
1) Define enemy types in an array. This will be like your enemy prefabs that you will not touch but only reference. The equivalent is the EnemyPool.
2) Populate an enemyArray by randomly selecting an enemy from enemyPool until the credit cost is 0.
3) Iterate through enemyArray and spawn enemies, placing them in waveArray. Once all the enemies in enemyArray have been spawned, clear it
4) Goto 2
*/

#include <math.h>
#include "WaveSystem.h"
#include "Particles.h"
#include "SoundManager.h"
#include "GameLoop.h"
#include "Win.h"

int currentWave=15;			
int enemiesKilled = 0;
#define MAXENEMYCOUNT 20							//The maximum amount of enemies that can be on screen at a time
EnemyInfo EnemiesToSpawn[MAXENEMYCOUNT];			//Populated in GenerateWave()
int enemySpawnIndex = 0;
EnemyInfo WaveObjects[WAVEOBJECTCOUNT];				//The main array containing all enemies in the wave
int enemyCount =0; 			//Keeps track of how many enemies are generated, loops back to index 0 when it exceeds WAVEOBJECTCOUNT
int waveCredits = 0; 		//Arbritarily set to currentWave * 10
int waveIndex = 0;			//Used to keep track of which enemy to spawn in the wavearray, loops back to index 0 when it exceeds WAVEOBJECTCOUNT


//Initialises the wave system, generates the first wave and subscribes turn events.
void InitWaveSystem(){
	//** Enemy Pool is initialised at the very start of the game so we do not need to initialise it here!
	GenerateWave();	
	SubscribeEvent(PLAYER_END,SpawnTombEnemies,1);
	SubscribeEvent(ZOMBIE_START,UpdateWave,0);
}

//Generates the wave by deducting credits and populating the array of enemies to spawn
void GenerateWave(){
	// Formula made in desmos, curve is a nice S curve.
	waveCredits = (int)(12.0f / (0.5f + powf(expf((float)-currentWave+10), 0.2f))); 

	while(waveCredits > 0){				
		//Gets random enemy index for prefab from enemystats.c
		//Index 0 is the wall so we exclude it from the random selection
		int randomEnemyIndex = CP_Random_RangeInt(1,GetEnemyCount()-1);

		//Check if Credits-Enemy cost is valid. (it loops and tries again if it isn't)
		//NOTE: Because there's no break, there MUST be an enemy with at least 1 for it's cost.
		if((waveCredits-GetEnemyPrefab(randomEnemyIndex)->Cost)>=0){
			//enemy%WAVEOBJECTCOUNT is so that the enemyCount index loops back around once it = WAVEOBJECTCOUNT
			//Ensure only the desire number of tombstone is generated and place in Waveobject array
				EnemiesToSpawn[enemySpawnIndex % MAXENEMYCOUNT] = *GetEnemyPrefab(randomEnemyIndex);
				EnemiesToSpawn[enemySpawnIndex % MAXENEMYCOUNT].is_Alive = FALSE;
				waveCredits -= GetEnemyPrefab(randomEnemyIndex)->Cost;
				enemySpawnIndex++;
		}
	}

}

//Update function for the wave enemies, handles their spawning and movement.
void UpdateWave(){
	//MOVES ENEMIES
	for(short y = 0; y < TOTAL_YGRID; ++y){
		for(short x = 0; x < TOTAL_XGRID; ++x){
			if(HasLiveEnemyInCell(x,y)){
				if(GetAliveEnemyFromGrid(x,y)->moveCooldown){
					GetAliveEnemyFromGrid(x,y)->moveCooldown = FALSE;
					continue;
				}
				if(GetAliveEnemyFromGrid(x,y)->MovementSpeed >0){
					MoveEnemy(GetAliveEnemyFromGrid(x,y));
				}
			}
		}
	}
	//=========================== SPAWNING OF ENEMIES HERE ===========================
	//Gets the number of enemies left to spawn
	int enemiesRemainingToSpawn = enemySpawnIndex-waveIndex;
	//Gets a random number that is either the column size or the enemies left to spawn, whichever is lesser.
	int randEnemyCount = CP_Random_RangeInt(1,min((TOTAL_YGRID-1),enemiesRemainingToSpawn));
	int currCount = 0;
	//As long as there are still enemies to spawn, we continue
	if(waveIndex < enemySpawnIndex){
		//We loop through all the enemies we CAN spawn
		while(currCount < randEnemyCount){
			SpawnEnemy(&EnemiesToSpawn[waveIndex]);
			currCount++;//gotta increment
			//If we finish spawning all the enemies, it means this wave has ended
			if(waveIndex >= enemySpawnIndex){
				NextWave();
				break;
			} 
		}
	}
	else{
		//extra catch just in case
		NextWave();
	}
	//Once all the possible spawns have been done, we switch over to players turn
	GameLoopSwitch(TURN_PLAYER);
}

//Displays enemies in the grid
void RenderEnemy(void){
	for(short i =0; i<WAVEOBJECTCOUNT; ++i){
		if(WaveObjects[i].is_Alive){
			DrawEnemy(&WaveObjects[i]);
		} 
	}
}

//Spawns a specific enemy in the specified cell if it is unoccupied. Used in tutorial for scripted spawns.
void SpawnEnemyInCell(int x, int y,EnemyInfo* enemy){
	if(HasLiveEnemyInCell(x,y)) return;
	for(short i = 0; i<WAVEOBJECTCOUNT; ++i){
		if(!WaveObjects[i].is_Alive){
		WaveObjects[i] = *enemy;
		WaveObjects[i].is_Alive = TRUE;
		WaveObjects[i].x = x;
		WaveObjects[i].y = y;
		break;
		}
	}
}

//Spawns enemies to the grid, used by update wave to spawn enemies into the grid.
void SpawnEnemy(EnemyInfo* enemy){
	//Grave enemies have a different spawning positon to other zombies.
	if(enemy->type == GRAVE) enemy->x = CP_Random_RangeInt(TOTAL_XGRID-2, TOTAL_XGRID-4);
	else enemy->x = TOTAL_XGRID-1;
	//The Y position of all zombies are randomised
	enemy->y = (CP_Random_RangeInt(0, TOTAL_YGRID - 1));
	if(HasLiveEnemyInCell(enemy->x,enemy->y))return;
	for(short i = 0; i<WAVEOBJECTCOUNT; ++i){
		if(!WaveObjects[i].is_Alive){
			enemy->is_Alive = TRUE;
			ZombieSpawnParticle(GridXToPosX(enemy->x),GridYToPosY(enemy->y));
			PlaySoundEx(ZOMBIESPAWN,CP_SOUND_GROUP_SFX);
			WaveObjects[i] = *enemy;
			waveIndex++;
			break;
		}
	}
}

//Creates a wall in unoccupied cell.
void CreateWall(int x, int y){
	if(HasLiveEnemyInCell(x,y))return;
	for(short i = 0; i<WAVEOBJECTCOUNT; ++i){
		if(!WaveObjects[i].is_Alive){
			WaveObjects[i] = *GetEnemyPrefab(0);
			WaveObjects[i].is_Alive = TRUE;
			WaveObjects[i].x = x;
			WaveObjects[i].y = y;
			PlaySound(WALLBUILD,CP_SOUND_GROUP_SFX);
			break;
		}
	}
}

//Spawns random enemy in cell occupied by grave enemies
void SpawnTombEnemies(void){
	for(short i=0; i<WAVEOBJECTCOUNT; ++i){
		if(HasLiveEnemyInCell(WaveObjects[i].x,WaveObjects[i].y)){
			if(WaveObjects[i].MovementSpeed <=0 &&WaveObjects[i].Cost >0){
				EnemyInfo newEnemy = *GetRandomEnemyPrefab();
				newEnemy.x = WaveObjects[i].x;	//we still need the xy pos of the tombstone
				newEnemy.y = WaveObjects[i].y;
				ZombieSpawnParticle(GridXToPosX(newEnemy.x),GridYToPosY(newEnemy.y));
				PlaySound(GRAVESPAWN,CP_SOUND_GROUP_SFX);
				WaveObjects[i] = newEnemy;		//replace tombstone with random enemy
				WaveObjects[i].is_Alive = TRUE;
				WaveObjects[i].moveCooldown = TRUE;
			}
		}
	}
}

//Iterates through grid, returns true if all enemies are dead, false if otherwise
BOOL IsAllEnemiesDead(){
	int count = 0;
	for(short i = 0; i<WAVEOBJECTCOUNT; ++i){
		if(WaveObjects[i].is_Alive && WaveObjects[i].Cost >0){
			count++;
		}
	}
	return (count ==0) ? TRUE:FALSE;
}


//Returns the live enemy in the specified grid. Returns NULL if otherwise.
EnemyInfo* GetAliveEnemyFromGrid(int x, int y){
	if(!HasLiveEnemyInCell(x,y))return NULL;
	for(short i=0; i< WAVEOBJECTCOUNT;++i){
		if((WaveObjects[i].x == x) && (WaveObjects[i].y ==y)){
			if(WaveObjects[i].is_Alive) return &WaveObjects[i];
		}
	}
	return NULL;
}

//Returns the specified cell. 
EnemyInfo* GetCell(int x, int y){
    for(short i=0; i< WAVEOBJECTCOUNT;++i){
        if((WaveObjects[i].x == x) && (WaveObjects[i].y ==y)){
            return &WaveObjects[i];
        }
    }
    return NULL;
}


//Checks if the enemy in the specified cell is alive.
BOOL HasLiveEnemyInCell(int x, int y){
	for(short i=0; i< WAVEOBJECTCOUNT;++i){
		if((WaveObjects[i].x == x) && (WaveObjects[i].y ==y)){
			if(WaveObjects[i].is_Alive) return TRUE;
		}
	}
	return FALSE;
}

//Returns the current wave count
int GetCurrentWave(){
	return currentWave;
}
//Returns the count of enemies killed
int GetEnemiesKilled(void){
	return enemiesKilled;
}

//Sends damage to enemy in specified cell. Called by tetromino pieces.
void SendDamage(int x, int y,int damage){
	//Make sure the thing we want to send damage to is in the playing area
	if(!IsInPlayingArea(GridXToPosX(x),GridYToPosY(y)))return;
	//Make sure it's not null/dead lol
	if (GetAliveEnemyFromGrid(x, y) == NULL) return;
	if (GetAliveEnemyFromGrid(x, y)->type == WALL) return;
	EnemyInfo* enemy = GetAliveEnemyFromGrid(x,y);
	enemy->Health-=damage;
	if(enemy->Health <=0){
		enemy->is_Alive = FALSE;
		ZombieDeathParticle(GridXToPosX(x),GridYToPosY(y),enemy->type);
		enemiesKilled++;
	}
}

//Function for zombies to deal damage to walls
void ZombieDealDamage(int x, int y,int damage){
	//Make sure the thing we want to send damage to is in the playing area
	if(!IsInPlayingArea(GridXToPosX(x),GridYToPosY(y)))return;
	//Make sure it's not null/dead lol
	if (GetAliveEnemyFromGrid(x, y) == NULL) return;
	EnemyInfo* enemy = GetAliveEnemyFromGrid(x,y);
	enemy->Health-=damage;
	if(enemy->Health <=0){
		enemy->is_Alive = FALSE;
		RadialParticleVaried(GridXToPosX(enemy->x),GridYToPosY(enemy->y));
	}
}

//Starts the next wave
void NextWave(){
	//Check if it's the last wave
	if(currentWave >= WAVES_TO_WIN) return;
	//reset array for enemies to spawn
	memset(EnemiesToSpawn,0,sizeof(EnemyInfo)*MAXENEMYCOUNT);
	waveIndex = 0;
	enemySpawnIndex =0;
	//Once the wave is done spawning, it waits for more turns
	currentWave++;
	GenerateWave();				//generates the next wave
}

//Resets the game state
void ResetGame(){
	//Reset wave index, enemyCount and the waveArray
	enemySpawnIndex = 0;
	waveIndex = 0;
	enemyCount = 0;
	currentWave = 0;
	enemiesKilled =0;
	memset(WaveObjects,0,sizeof(EnemyInfo)*(WAVEOBJECTCOUNT));
	memset(EnemiesToSpawn,0,sizeof(EnemyInfo)*MAXENEMYCOUNT);
}