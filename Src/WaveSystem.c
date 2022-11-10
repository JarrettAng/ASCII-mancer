#include "WaveSystem.h"
#include "Particles.h"
#include "SoundManager.h"
#include "GameLoop.h"
int currentWave=1;			
int enemyThreshold = 10;			//Set to a ratio of the current wave credits (used later)

EnemyInfo WaveObjects[WAVEOBJECTCOUNT];	//Max size of the array probably = gridsize. I don't think we'll have more enemies than that
// EnemyInfo TombstoneObjects[TOMBSTONECOUNT]; // Max size of tombstone avilable on grid
int tombstoneObjectCount = 0;
int enemyCount =0; 			//Keeps track of how many enemies are generated, loops back to index 0 when it exceeds WAVEOBJECTCOUNT
int tombstoneCount = 0;			//Keep track of how many tombstone are there in the grid currently
int waveCredits = 0; 		//Arbritarily set to currentWave * 10
int spawnInterval = 0;		//How much time in between spawns. (Time referring to turns.)
int waveIndex = 0;			//Used to keep track of which enemy to spawn in the wavearray, loops back to index 0 when it exceeds WAVEOBJECTCOUNT
int spawnChance = 4;		//this creates a 1/(num+1) chance of spawning enemies in a col
int spawnTimer = 0;	//Turns left before it will spawn. Checks against spawn interval
//Enemy Array, cost, contains all available enemies
//Wave array, Filled with enemies based off of cost.
int x_SpawnPos = TOTAL_XGRID - 1;//Enemy x-grid spawn position
int ts_MaxX = 9;
int ts_MinX = 5;
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

	SubscribeEvent(ZOMBIE_START,UpdateWave,0);
}
//Generates the wave by deducting credits and adding enemies to the wave
void GenerateWave(){
	waveCredits = currentWave *4;								//magic number, will tweak
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
			if (WaveObjects[i].is_Alive)
			{
				MoveEnemy(&WaveObjects[i]);
			}
		}
		//Iterates through the wave array and starts spawning the enemies
		if((waveIndex) < (enemyCount)){					
			//Loop through the Y axis of the grid
			for(short y = 0; y<TOTAL_YGRID-1;++y){
			int randNum = CP_Random_RangeInt(0,spawnChance);
				if(!randNum){ //20% chance!
					SpawnEnemy(&WaveObjects[waveIndex%WAVEOBJECTCOUNT]);
					//Since waveindex is incremented on spawn, we need to check if it's finished
					if ((waveIndex) >= (enemyCount)) break;
				}
				else{
					//If no enemy has been spawned, increase the chance of spawning.
					spawnChance--;
				}
			}
		}
		else{
			NextWave();
			spawnChance = 4;
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
void SpawnEnemy(EnemyInfo* enemy){
	//SET ENEMY X VALUES
	//if the enemy is a tombstone
	if(enemy->Cost ==0){
		waveIndex++;
		return;
	}
	if(enemy->MovementSpeed <=0){
		enemy->x = CP_Random_RangeInt(TOTAL_XGRID-2, TOTAL_XGRID-4);
	}//else it's a normal zombie
	else{
		enemy->x = TOTAL_XGRID-1;
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

EnemyInfo* GetEnemyFromGrid(int x, int y){
	for(short i=0; i< WAVEOBJECTCOUNT;++i){
		if((WaveObjects[i].x == x) && (WaveObjects[i].y ==y)){
			return &WaveObjects[i];
		}
	}
}

// void SpawnTombEnemies(void){
// 	for(short i=0; i<(enemyCount%WAVEOBJECTCOUNT); ++i){
// 		 if(WaveObjects[i].Health <=0)
// 		 {
// 			continue;
// 		 }
// 		if(WaveObjects[i].is_Alive){
// 			if(WaveObjects[i].MovementSpeed ==0 && WaveObjects[i].Cost > 0){
// 				WaveObjects[i] = *GetRandomEnemyPrefab();
// 			}
// 		}
// 	}
// }

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

//Function to send damage to enemy in grid
void SendDamage(int x, int y,int damage){
	//Make sure the thing we want to send damage to is in the playing area
	if(!IsInPlayingArea(GridXToPosX(x),GridYToPosY(y)))return;
	//Make sure it's not null/dead lol
	if (GetAliveEnemyFromGrid(x, y) == NULL) return;
	if(GetAliveEnemyFromGrid(x, y)->Cost == 0) return;
	EnemyInfo* enemy = GetAliveEnemyFromGrid(x,y);
	enemy->Health-=damage;
	if(enemy->Health <=0){
		enemy->is_Alive = FALSE;
		ZombieDeathParticle(GridXToPosX(x),GridYToPosY(y));
	}
}
void ZombieDealDamage(int x, int y,int damage){
	//Make sure the thing we want to send damage to is in the playing area
	if(!IsInPlayingArea(GridXToPosX(x),GridYToPosY(y)))return;
	//Make sure it's not null/dead lol
	if (GetAliveEnemyFromGrid(x, y) == NULL) return;
	EnemyInfo* enemy = GetAliveEnemyFromGrid(x,y);
	enemy->Health-=damage;
	if(enemy->Health <=0){
		enemy->is_Alive = FALSE;
		ZombieDeathParticle(GridXToPosX(x),GridYToPosY(y));
	}
}

void NextWave()
{
	//Once the wave is done spawning, it waits for more turns
	spawnTimer = spawnInterval*3; //arbritrary number, just makes it so it takes longer between each wave
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