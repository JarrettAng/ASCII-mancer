#include <stdio.h>
#include "WaveSystem.h"

int current_Wave;
int enemy_Threshold;

void UpdateWaveStatus()
{
	//For now, the condition for next wave is checking how many are there left on the screen
	if (enemy_Left < enemy_Threshold)
	{
		NextWave();
	}
}

void NextWave()
{
	current_Wave++;
	///
	//Put all the needed function for the new wave here
	///
}