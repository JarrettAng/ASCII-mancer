#include "WaveSystem.h"
#include "EnemyStats.h"

int currentWave=1;			
int enemyThreshold;			//Set to a ratio of the current wave credits (used later)

EnemyInfo WaveObjects[200];	//Max size of the array probably = gridsize. I don't think we'll have more enemies than that
int enemyCount =0; 			//Keeps track of how many enemies are generated
int waveCredits = 0; 		//Arbritarily set to currentWave * 10
int spawnInterval = 1;		//How much time in between spawns. (Time referring to turns.)
int waveIndex = 0;			//Used to keep track of which enemy to spawn in the wavearray

int spawnTimer = 0;			//Turns left before it will spawn. Checks against spawn interval
//Enemy Array, cost, contains all available enemies
//Wave array, Filled with enemies based off of cost.

/*
@Brief (just in case anyone wants to know the logic)
1) Define enemy types in an array. This will be like your enemy prefabs that you will not touch but only reference. The equivalent is the EnemyPool.
2) Populate a waveArray by randomly selecting an enemy from enemyPool until the credit cost is 0.
3) Iterate through the waveArray and spawn. Since we cannot removeAt(index), we have to manually track it and once the index hits the total enemy count, it means we've spawned all enemies and we can generate the next wave. Goto 2
*/

//TODO : Probably need to do some object pooling with the wavearray.

//Initialises the wave system
void InitWaveSystem(){
	InitEnemyPool();
	GenerateWave();
}


//Generates the wave by deducting credits and adding enemies to the wave
void GenerateWave(){
	waveCredits = currentWave *5;					//magic number
	while(waveCredits > 0){
		//Gets a random index from the pool of enemies (declared in enemystats.c)
		int randomEnemyIndex = CP_Random_RangeInt(0,GetEnemyCount()-1);
		//Check if Credits-Enemy cost is valid. (it loops and tries again if it isn't)
		//NOTE: Because there's no break, there MUST be an enemy with at least 1 for it's cost.
		if((waveCredits-GetEnemy(randomEnemyIndex)->Cost)>=0)		
		{
			WaveObjects[enemyCount] = *GetEnemy(randomEnemyIndex);
			waveCredits-=GetEnemy(randomEnemyIndex)->Cost;
			enemyCount++;
		}
		/*
		else if(waveCredits <= 0) break;		//I think not needed
		*/
	}

}


//Update function for the wave system
void UpdateWave(){
	//For now, the condition for next wave is checking how many are there left on the screen
	if(CP_Input_KeyTriggered(KEY_K)) //temp to slowly increment the wave 
	{
		//SPAWN ENEMIES
		if(spawnTimer > 0){			//Should be replaced with some variation of turn counter.
			spawnTimer--;
			return;
		} 
		//Iterates through the wave array and starts spawning the enemies
		if(waveIndex < enemyCount){
			//Can probably be wrapped to a spawn enemy function
			WaveObjects[waveIndex].x = (float)CP_System_GetWindowWidth()-10.f;
			float tempCellYSize = (float)(CP_System_GetWindowHeight()/9);
			float randomYOffset = (float)(CP_Random_RangeInt(0,9)*tempCellYSize);
			WaveObjects[waveIndex].y = randomYOffset;
			WaveObjects[waveIndex].is_Alive = TRUE;
			waveIndex++;
			spawnTimer = spawnInterval;		//Makes it such that enemies turn by turn
		}
		else{
			NextWave();
		}

		//MOVE ENEMIES
		//int liveEnemyCount = 0;
		for(short i =0; i<waveIndex;++i)		//will probably refactor this
		{
			if(WaveObjects[i].is_Alive)
			{
				MoveEnemy(&WaveObjects[i]);
				// liveEnemyCount++;
			}
			// if(i == waveIndex-1 && liveEnemyCount ==0)
			// {

			// }
		}
	}
	//DISPLAY ENEMIES
	for(short i =0; i<waveIndex; ++i)		//will probably refactor this
	{
		if(WaveObjects[i].is_Alive)
			{
				DrawEnemy(&WaveObjects[i]);
			}
	}
	
}

void NextWave()
{
	//Once the wave is done spawning, it waits for more turns
	spawnTimer = spawnInterval*2; //arbritrary number, just makes it so it takes longer between each wave
	currentWave++;
	GenerateWave();				//generates the next wave
}

void ClearWaveArray(){
	memset(WaveObjects,0,sizeof(EnemyInfo)*(9*23));
}