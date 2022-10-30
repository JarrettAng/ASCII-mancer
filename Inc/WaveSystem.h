#pragma once
#include <cprocessing.h>

//Initialises the wave system. MUST BE CALLED AT LEAST ONCE.
void InitWaveSystem(void);
//Generates the wave using wave credits and populates wave array
void GenerateWave(void);
//Updates the position of all the enemies in the wave.
void UpdateWave(void);
//Function that increments wave and generates the next one
void NextWave(void);

//Unused for now
void ClearWaveArray(void);
