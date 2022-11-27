/*!
@file	  WaveSystem.h
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@date     21/11/2022
@brief    This header file contains the information about the wavesystem and the functions that can be called for it. The specifics of the wavesystem algorithm is detailed in WaveSystem.c

@license  Copyright © 2022 DigiPen, All rights reserved.
________________________________________________________________________________________________*/

#pragma once

#include <cprocessing.h>
#include "EnemyStats.h"
#include "Grid.h"

//Total count of all possible enemies in the grid with a bit extra as a buffer
#define WAVEOBJECTCOUNT (TOTAL_XGRID*TOTAL_YGRID*3)
/*____________________________________________________________________*/

/*
@brief Initialises the wave system. MUST BE CALLED AT LEAST ONCE.
*/
void InitWaveSystem(void);
/*____________________________________________________________________*/

/*
@brief Generates the wave using wave credits and populates wave array
*/
void GenerateWave(void);
/*____________________________________________________________________*/

/*
@brief Updates the position of all the enemies in the wave.
*/
void UpdateWave(void);
/*____________________________________________________________________*/

/*
@brief Starts the next wave.
*/
void NextWave(void);
/*____________________________________________________________________*/

/*
@brief Creates a wall in the specified cell if it's unoccupied.
@param[in] x,y Index of cell to spawn wall in
*/
void CreateWall(int x, int y);
/*____________________________________________________________________*/

/*
@brief Gets the live enemy in the specified cell. NULL if otherwise.
@param[in] x,y Index of cell to get live enemy in
*/
EnemyInfo* GetAliveEnemyFromGrid(int x, int y);
/*____________________________________________________________________*/

/*
@brief Checks if there is a live enemy in the specified cell
@param[in] x,y Index of cell to check
*/
_Bool HasLiveEnemyInCell(int x, int y);
/*____________________________________________________________________*/

/*
@brief Sends specified damage to specified cell
@param[in] x,y Index of cell to send damage to
@param[in] damage is the amount of damage to send
*/
void SendDamage(int x, int y, int damage);
/*____________________________________________________________________*/

/*
@brief Function for zombies to send damage to other zombies.
@param[in] x,y Index of cell to send damage to
@param[in] damage is the amount of damage to send
*/
void ZombieDealDamage(int x, int y,int damage);
/*____________________________________________________________________*/

/*
@brief Spawns random enemy at cell occupied by grave enemy
*/
void SpawnTombEnemies(void);
/*____________________________________________________________________*/

/*
@brief Returns the total number of enemies killed this game
*/
int GetEnemiesKilled(void);
/*____________________________________________________________________*/

/*
@brief Returns the current wave count
*/
int GetCurrentWave(void);
/*____________________________________________________________________*/

/*
@brief Spawns specified enemy (used in UpdateWave to spawn enemies to the grid)
@param[in] enemy specific enemy to be spawned. Position is based random and based off of the enemy type.
*/
void SpawnEnemy(EnemyInfo* enemy);
/*____________________________________________________________________*/

/*
@brief Spawns specified enemy to the cell specified if it is unoccupied
@param[in] x,y specified cell to spawn enemy in if it is unoccupied.
@param[in] enemy specific enemy to be spawned.
*/
void SpawnEnemyInCell(int x, int y,EnemyInfo* enemy);
/*____________________________________________________________________*/

/*
@brief Checks if all the enemies in the grid are dead.
*/
_Bool IsAllEnemiesDead(void);
/*____________________________________________________________________*/

/*
@brief Renders all live enemies in the grid
*/
void RenderEnemy(void);
/*____________________________________________________________________*/

/*
@brief Gets the enemy from the specified grid cell. Returns NULL if no enemy in cell
@param[in] x,y the index of the specified cell to check. 
*/
EnemyInfo* GetEnemyFromGrid(int x, int y);
/*____________________________________________________________________*/

/*
@brief Resets the game stats and clears the arrays to be repopulated again.
*/
void ResetGame(void);
/*____________________________________________________________________*/

/*
@brief Debug mode function to set the wave
*/
void SetWave(int wave);