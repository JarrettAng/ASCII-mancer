#include "WaveSystem.h"
#include "Particles.h"
#include "SoundManager.h"
int currentWave=1;			
int enemyThreshold = 10;			//Set to a ratio of the current wave credits (used later)

EnemyInfo WaveObjects[WAVEOBJECTCOUNT];	//Max size of the array probably = gridsize. I don't think we'll have more enemies than that
EnemyInfo TombstoneObjects[TOMBSTONECOUNT]; // Max size of tombstone avilable on grid
int tombstoneObjectCount = 0;
int enemyCount =0; 			//Keeps track of how many enemies are generated, loops back to index 0 when it exceeds WAVEOBJECTCOUNT
int tombstoneCount = 0;			//Keep track of how many tombstone are there in the grid currently
int waveCredits = 0; 		//Arbritarily set to currentWave * 10
int spawnInterval = 0;		//How much time in between spawns. (Time referring to turns.)
int waveIndex = 0;			//Used to keep track of which enemy to spawn in the wavearray, loops back to index 0 when it exceeds WAVEOBJECTCOUNT

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
	InitEnemyPool();				//Starts up the enemy prefabs array
	GenerateWave();	
}
//Generates the wave by deducting credits and adding enemies to the wave
void GenerateWave(){
	waveCredits = currentWave *4;								//magic number, will tweak
	while(waveCredits > 0 && enemyThreshold <=10){				//Spawn as long as we have credits or lesser than 10 enemies to spawn
		//Gets random enemy index for prefab from enemystats.c
		int randomEnemyIndex = CP_Random_RangeInt(0,GetEnemyCount()-1);
		//Check if Credits-Enemy cost is valid. (it loops and tries again if it isn't)
		//NOTE: Because there's no break, there MUST be an enemy with at least 1 for it's cost.
		if((waveCredits-GetEnemyPrefab(randomEnemyIndex)->Cost)>=0){
			//enemy%WAVEOBJECTCOUNT is so that the enemyCount index loops back around once it = WAVEOBJECTCOUNT
			//Ensure only the desire number of tombstone is generated and place in Waveobject array
			if (randomEnemyIndex == 3 && tombstoneObjectCount == TOMBSTONECOUNT)
			{
				continue;
			}
			else
			{
				if (randomEnemyIndex == 3)
				{
					tombstoneObjectCount++;
				}
				WaveObjects[enemyCount % WAVEOBJECTCOUNT] = *GetEnemyPrefab(randomEnemyIndex);
				WaveObjects[enemyCount % WAVEOBJECTCOUNT].is_Alive = FALSE;
				waveCredits -= GetEnemyPrefab(randomEnemyIndex)->Cost;
				enemyCount++;
			}
		}
	}

}
//int GetTombstonePosition()
//{
//
//}

//Update function for the wave system
//Probably do void Subscribe_ZombieTurnUpdate(UpdateWave);
void UpdateWave(){

	if(CP_Input_KeyTriggered(KEY_K)) //temp to slowly increment the wave 
	{
		//SPAWN ENEMIES
		if(spawnTimer > 0){			//TODO: Should be replaced with some variation of turn counter.
			spawnTimer--;
			return;
		}
		else{
			//MOVE ENEMIES
			for(short i =0; i<WAVEOBJECTCOUNT;++i)
			{
				if(WaveObjects[i].is_Alive)
				{
					MoveEnemy(&WaveObjects[i]);
				}
			}
		} 
		//Iterates through the wave array and starts spawning the enemies
		if((waveIndex) < (enemyCount)){					
			//Loop through the Y axis of the grid
			for(short y = 0; y<TOTAL_YGRID-1;++y){
				int randNum = CP_Random_RangeInt(0,4);
				if(!randNum){ //20% chance!
					//Check if object being spawn is a tombstone or enemy
					if (WaveObjects[waveIndex % WAVEOBJECTCOUNT].CharSprite == "4" && tombstoneCount <= TOMBSTONECOUNT)
					{
						//Spawn position Tombstone
						x_SpawnPos = CP_Random_RangeInt(ts_MinX, ts_MaxX);
						WaveObjects[waveIndex % WAVEOBJECTCOUNT].x = x_SpawnPos;
						tombstoneCount++;//TEMP

						//MENEMY SPAWN AT TOMBSTONE POSITION W.I.P
						//for (int i = 0; i < TOMBSTONECOUNT; ++i)
						//{
						//	if (!TombstoneObjects[i].is_Alive)
						//	{
						//		TombstoneObjects[i] = WaveObjects[waveIndex % WAVEOBJECTCOUNT];//Store tombstone in their own array
						//		tombstoneCount++;
						//		break;
						//	}
						//}
					}
					else
					{
						//Decide where enemy should spawn
						//int spawnPosChance = CP_Random_RangeInt(0, 4);
						//if (!spawnPosChance && tombstoneCount >0)
						//{
						//	int ts_index = GetTombstonePosition();
						//	x_SpawnPos = TombstoneObjects[ts_index].x;
						//	WaveObjects[waveIndex % WAVEOBJECTCOUNT].x = x_SpawnPos;
						//}
						//else
						{
							x_SpawnPos = TOTAL_XGRID - 1;
							WaveObjects[waveIndex % WAVEOBJECTCOUNT].x = x_SpawnPos;			//Start at far right
						}
					}
					int enemyStartY = (CP_Random_RangeInt(0, TOTAL_YGRID - 1));			//Spawn at random y pos
					//We have to make sure the cell is not already occupied before spawning
					if (!HasLiveEnemyInCell(WaveObjects[waveIndex % WAVEOBJECTCOUNT].x, enemyStartY)) {
						WaveObjects[waveIndex % WAVEOBJECTCOUNT].y = enemyStartY;
						WaveObjects[waveIndex % WAVEOBJECTCOUNT].is_Alive = TRUE;
						//Spawn particle & play sound
						ZombieSpawnParticle(GridXToPosX(WaveObjects[waveIndex % WAVEOBJECTCOUNT].x), GridYToPosY(WaveObjects[waveIndex % WAVEOBJECTCOUNT].y));
						PlaySoundEx(ZOMBIESPAWN, CP_SOUND_GROUP_SFX);
						waveIndex++;
						//if waveIndex == enemycount (or exceeds) it means we finished spawning all enemies in this wave
						if ((waveIndex) >= (enemyCount)) break;
					}
				}
			}
			spawnTimer = spawnInterval;	//Makes it such that enemies turn by turn
		}
		else{
			NextWave();
			//Reset value for tombstone
			//tombstoneObjectCount = 0;
			//tombstoneCount = 0;
		}
	}

	//DISPLAY ENEMIES
	for(short i =0; i<WAVEOBJECTCOUNT; ++i)		
	{
		if(WaveObjects[i].is_Alive){
			DrawEnemy(&WaveObjects[i]);
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

//Function to send damage to enemy in grid
void SendDamage(int x, int y,int damage){
	//Make sure the thing we want to send damage to is in the playing area
	if(!IsInPlayingArea(GridXToPosX(x),GridYToPosY(y)))return;
	//Make sure it's not null/dead lol
	if (GetAliveEnemyFromGrid(x, y) == NULL) return;
	EnemyInfo* enemy = GetAliveEnemyFromGrid(x,y);
	enemy->Health-=damage;
	if(enemy->Health <=0){
		enemy->is_Alive = FALSE;
		if (enemy->CharSprite == "4")
		{
			tombstoneCount--;
		}
		ZombieDeathParticle(GridXToPosX(x),GridYToPosY(y));
	}
}

void NextWave()
{
	//Once the wave is done spawning, it waits for more turns
	spawnTimer = spawnInterval*3; //arbritrary number, just makes it so it takes longer between each wave
	currentWave++;
	//ClearWaveArray();
	GenerateWave();				//generates the next wave
}

void ClearWaveArray(){
	memset(WaveObjects,0,sizeof(EnemyInfo)*(WAVEOBJECTCOUNT));
}