#include "WaveSystem.h"
#include "Particles.h"
#include "SoundManager.h"
#include "GameLoop.h"
int currentWave=1;			
int enemyThreshold = 10;			//Set to a ratio of the current wave credits (used later)

int enemiesKilled = 0;

EnemyInfo WaveObjects[WAVEOBJECTCOUNT];	//Max size of the array probably = gridsize. I don't think we'll have more enemies than that
int tombstoneObjectCount = 0;
int enemyCount =0; 			//Keeps track of how many enemies are generated, loops back to index 0 when it exceeds WAVEOBJECTCOUNT
int tombstoneCount = 0;			//Keep track of how many tombstone are there in the grid currently
int waveCredits = 0; 		//Arbritarily set to currentWave * 10
int waveIndex = 0;			//Used to keep track of which enemy to spawn in the wavearray, loops back to index 0 when it exceeds WAVEOBJECTCOUNT
//Wave array, Filled with enemies based off of cost.
int x_SpawnPos = TOTAL_XGRID - 1;//Enemy x-grid spawn position
/*
@Brief (just in case anyone wants to know the logic)
1) Define enemy types in an array. This will be like your enemy prefabs that you will not touch but only reference. The equivalent is the EnemyPool.
2) Populate a waveArray by randomly selecting an enemy from enemyPool until the credit cost is 0.
3) Iterate through the waveArray and spawn. Since we cannot removeAt(index), we have to manually track it and once the index hits the total enemy count, it means we've spawned all enemies and we can generate the next wave. Goto 2
*/


//Initialises the wave system
void InitWaveSystem(){
	//InitEnemyPool();				//Starts up the enemy prefabs array
	GenerateWave();	
	// UpdateWave();
	SpawnEnemy(&WaveObjects[waveIndex%WAVEOBJECTCOUNT]);
	SubscribeEvent(PLAYER_END,SpawnTombEnemies,1);
	SubscribeEvent(ZOMBIE_START,UpdateWave,0);
}
//Generates the wave by deducting credits and adding enemies to the wave
void GenerateWave(){
	waveCredits = currentWave;								//magic number, will tweak
	while(waveCredits > 0 && enemyThreshold <=10){				//Spawn as long as we have credits or lesser than 10 enemies to spawn
		//Gets random enemy index for prefab from enemystats.c
		//1 is Wall so do not spawn it
		int randomEnemyIndex = CP_Random_RangeInt(1,GetEnemyCount()-1);
		//Check if Credits-Enemy cost is valid. (it loops and tries again if it isn't)
		//NOTE: Because there's no break, there MUST be an enemy with at least 1 for it's cost.

		if((waveCredits-GetEnemyPrefab(randomEnemyIndex)->Cost)>=0){
			//enemy%WAVEOBJECTCOUNT is so that the enemyCount index loops back around once it = WAVEOBJECTCOUNT
			//Ensure only the desire number of tombstone is generated and place in Waveobject array
				WaveObjects[enemyCount % WAVEOBJECTCOUNT] = *GetEnemyPrefab(randomEnemyIndex);
				WaveObjects[enemyCount % WAVEOBJECTCOUNT].is_Alive = FALSE;
				waveCredits -= GetEnemyPrefab(randomEnemyIndex)->Cost;
				enemyCount++;

		}
	}

}

//Update function for the wave system, subscribed to gameloop enemy wave
void UpdateWave(){
		//MOVE ENEMIES
		for (short i = 0; i < WAVEOBJECTCOUNT; ++i)
		{
			if(WaveObjects[i].moveCooldown){			//check for enemies just spawned in by tomb enemies
				WaveObjects[i].moveCooldown = FALSE;
				continue;
			}
			if (WaveObjects[i].is_Alive && WaveObjects[i].MovementSpeed >0)
			{
				MoveEnemy(&WaveObjects[i]);
			}
		}
		//Gets the number of enemies left to spawn
		int enemiesRemainingToSpawn = enemyCount-waveIndex;
		//Gets a random number that is either the column size or the enemies left to spawn, whichever is lesser.
		int randEnemyCount = CP_Random_RangeInt(1,min((TOTAL_YGRID-1),enemiesRemainingToSpawn));
		int currCount = 0;
		//Gets a random position in Y axis, accounting for randEnemyCount
		int randYPosBuffer = CP_Random_RangeInt(0,(TOTAL_YGRID-1)-randEnemyCount);
		if(waveIndex < enemyCount){
			//We loop through all the enemies we wanna spawn
			while(currCount < randEnemyCount){
				//we skip the walls
				if (WaveObjects[waveIndex % WAVEOBJECTCOUNT].Cost == 0) {
					waveIndex++;
				}
				//Spawn the enemies
				SpawnEnemy(&WaveObjects[waveIndex%WAVEOBJECTCOUNT]);
				currCount++;	//gotta increment
				//If we hit the maxenemycount, it means this wave has ended
				if(waveIndex >= enemyCount){
					NextWave();
					break;
				} 
			}
		}
		else{
			//extra catch just in case
			NextWave();
		}
	GameLoopSwitch(TURN_PLAYER);
}

//DISPLAY ENEMIES
void RenderEnemy(void){
	for(short i =0; i<WAVEOBJECTCOUNT; ++i)		
	{
		if(WaveObjects[i].is_Alive){
			DrawEnemy(&WaveObjects[i]);
		} 
	}
}

//Tries to spawn enemy in random grid square. If it fails, whileloop tries again.
void SpawnEnemy(EnemyInfo* enemy){
	//SET ENEMY X VALUES
	//if the enemy is a wall, skip it don't spawn
	if(enemy->type == WALL){
		waveIndex++;
		return;
	}
	//If the enemy is a tombstone, spawn it between index 9 to 11
	if(enemy->type == GRAVE){
		enemy->x = CP_Random_RangeInt(TOTAL_XGRID-2, TOTAL_XGRID-4);
	}
	//else it's a normal zombie
	else{
		enemy->x = TOTAL_XGRID-1;		//this puts it outside of the grid
	}
	//SET ENEMY Y VALUES
	int randomYPos = (CP_Random_RangeInt(0, TOTAL_YGRID - 1));			//Spawn at random y pos
	if(!HasLiveEnemyInCell(enemy->x,randomYPos)){
		enemy->y = randomYPos;
		enemy->is_Alive = TRUE;				//This controls if the enemy is "spawned" or not.
		ZombieSpawnParticle(GridXToPosX(enemy->x),GridYToPosY(enemy->y));
		PlaySoundEx(ZOMBIESPAWN,CP_SOUND_GROUP_SFX);
		waveIndex++;
	}
}

void CreateWall(int x, int y){
	if(!HasLiveEnemyInCell(x,y)){
		WaveObjects[enemyCount % WAVEOBJECTCOUNT] = *GetEnemyPrefab(0);
		WaveObjects[enemyCount % WAVEOBJECTCOUNT].is_Alive = TRUE;
		WaveObjects[enemyCount % WAVEOBJECTCOUNT].x = x;
		WaveObjects[enemyCount % WAVEOBJECTCOUNT].y = y;
		enemyCount++;
		PlaySound(WALLBUILD,CP_SOUND_GROUP_SFX);
	}
}

void SpawnTombEnemies(void){
	for(short i=0; i<WAVEOBJECTCOUNT; ++i){
		if(HasLiveEnemyInCell(WaveObjects[i].x,WaveObjects[i].y)){
			if(WaveObjects[i].MovementSpeed <=0 &&WaveObjects[i].Cost >0){
				EnemyInfo newEnemy = *GetRandomEnemyPrefab();
				newEnemy.x = WaveObjects[i].x;	//we still need the xy pos of the tombstone
				newEnemy.y = WaveObjects[i].y;
				ZombieSpawnParticle(GridXToPosX(newEnemy.x),GridYToPosY(newEnemy.y));
				WaveObjects[i] = newEnemy;		//replace tombstone with random enemy
				WaveObjects[i].is_Alive = TRUE;
				WaveObjects[i].moveCooldown = TRUE;
			}
		}
	}
}

//Returns address of live enemy in the grid
EnemyInfo* GetAliveEnemyFromGrid(int x, int y){
	for(short i=0; i< WAVEOBJECTCOUNT;++i){
		if((WaveObjects[i].x == x) && (WaveObjects[i].y ==y)){
			if(WaveObjects[i].is_Alive) return &WaveObjects[i];
		}
	}
	return NULL;
}


BOOL HasLiveEnemyInCell(int x, int y){
	for(short i=0; i< WAVEOBJECTCOUNT;++i){
		if((WaveObjects[i].x == x) && (WaveObjects[i].y ==y)){
			if(WaveObjects[i].is_Alive) return TRUE;
		}
	}
	return FALSE;
}

int GetCurrentWave(){
	return currentWave;
}
int GetEnemiesKilled(void){
	return enemiesKilled;
}

//Function to send damage to enemy in grid
void SendDamage(int x, int y,int damage){
	//Make sure the thing we want to send damage to is in the playing area
	if(!IsInPlayingArea(GridXToPosX(x),GridYToPosY(y)))return;
	//Make sure it's not null/dead lol
	if (GetAliveEnemyFromGrid(x, y) == NULL) return;
	if(GetAliveEnemyFromGrid(x, y)->type == WALL) return;
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

	}
}

void NextWave()
{
	//Once the wave is done spawning, it waits for more turns
	currentWave++;
	GenerateWave();				//generates the next wave
}

//Resets the game state
void ResetGame(){
	//Reset wave index, enemyCount and the waveArray
	waveIndex = 0;
	enemyCount = 0;
	currentWave = 0;
	memset(WaveObjects,0,sizeof(EnemyInfo)*(WAVEOBJECTCOUNT));
}