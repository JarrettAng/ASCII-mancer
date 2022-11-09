#pragma once
#include <cprocessing.h>
#include "EnemyStats.h"
#include "Grid.h"

#define WAVEOBJECTCOUNT 20
#define TOMBSTONECOUNT 3
//Initialises the wave system. MUST BE CALLED AT LEAST ONCE.
void InitWaveSystem(void);
//Generates the wave using wave credits and populates wave array
void GenerateWave(void);
//Updates the position of all the enemies in the wave.
void UpdateWave(void);
//Function that increments wave and generates the next one
void NextWave(void);
EnemyInfo* GetAliveEnemyFromGrid(int x, int y);
BOOL HasLiveEnemyInCell(int x, int y);
void SendDamage(int x, int y, int damage);
int GetCurrentWave(void);
// EnemyInfo* GetEnemyFromGrid(int x, int y);
//Unused for now
void ClearWaveArray(void);