//#include <stdio.h>
#include "WaveSystem.h"
#include "EnemyStats.h"

int currentWave;
int enemyThreshold;	//Set to a ratio of the current wave credits 

EnemyInfo WaveObjects[9*23];
int enemyCount =0; 			//Keeps track of how many enemies are generated
int waveCredits = 0; 		//Arbritarily set to currentWave * 10
int SpawnInterval = 1;		//How much time in between spawns. (Time referring to turns.)

//Enemy Array, cost, contains all available enemies
//Wave array, Filled with enemies based off of cost.


//Generates the wave by deducting credits and adding enemies to the wave
void GenerateWave(){
	waveCredits = currentWave *10;
	while(waveCredits > 0|| enemyCount<10){
		int randomEnemyIndex = CP_Random_RangeInt(0,enemyIndex);
		int randomEnemyCost = Enemy[randomEnemyIndex].Cost;
		if((waveCredits-randomEnemyCost)>=0)
		{
			WaveObjects[enemyCount] = Enemy[randomEnemyIndex];
			waveCredits-=Enemy[randomEnemyIndex].Cost;
			enemyCount++;
		}
		else if(waveCredits<=0) break;
	}

}
void UpdateWaveStatus()
{
	//For now, the condition for next wave is checking how many are there left on the screen
	if (enemy_Left < enemyThreshold)
	{
		NextWave();
	}
}

void NextWave()
{
	currentWave++;
	///
	//Put all the needed function for the new wave here
	///
}