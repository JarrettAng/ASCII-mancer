//Header files here
#include <stdio.h>

#include "Grid.h"
#include "Particles.h"
#include "WaveSystem.h"
#include "Screenshake.h"
#include "Hearts.h"
#include "SoundManager.h"
#include "gamelevel.h"
#include "Wizard.h"
#include "Win.h"

#include "GameLoop.h" // For event system, calls event subscribed to player turn start, end, etc

#include "Tutorial.h" // For initializing how to play system that shows on first turn of player

#include "TManager.h" // For initializing everything about Tetris Pieces
#include "TPlayer.h" // For handling player's input with Tetris Pieces

#include "EnemyDisplay.h" // For drawing the enemy stats on the grid

void gameLevelInit(void){
	// Zero out all the events in the event system first!
	ZeroOutAllEvents();

	PlayBGM(GAMEBGM);
	grid_init();

	InitWaveSystem();
	InitializeLife();
	InitWizard();
	WinTextInit();

	TManagerInit(DEFAULT_SPAWN_SEED);

	TPlayerInit();
	EnemyDisplayInit();

	TutorialInit();

	// Initialize game loop last, after all the events have been subscribed
	GameLoopInit();

}
void gameLevelUpdate(void){
	UpdateCameraShaker();

	// DRAWING AND UPDATING OF GRID
	grid_update();
	if (CP_Input_KeyTriggered(KEY_K)){
		CreateWall(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()));
		// SpawnTombEnemies();
	}
	if (CP_Input_KeyTriggered(KEY_V)){
		SpawnEnemyInCell(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()),GetEnemyPrefab(1));
	}

	// Call the functions subscribed to the update events
	GameLoopUpdate();

	//]UpdateWave();
	RenderEnemy();
	// UPDATE VFX
	UpdateEffects();
	RenderHand();
	ShowCurrentWave();
	//ShowTestEnemiesKilled();
	
	// UPDATE WIZARD
	UpdateWizard();
	// UPDATE HEARTS
	UpdateLife();
	// CHECK IF WON
	CheckWinCondition();
	// CLEAR SCREEN
	CP_Graphics_ClearBackground(BLACK);

}

void ShowCurrentWave(void){
	CP_Settings_Fill(TETRIS_COLOR);
	CP_Settings_TextSize(CP_System_GetWindowHeight() / 20.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);
	char buffer[25] = { 0 };
	sprintf_s(buffer, 25, "WAVE %02d/%02d", GetCurrentWave(), WAVES_TO_WIN);
	float xPosition = WINDOWLENGTH - GetCellSize();
	float yPosition = GetCellSize();
	CP_Font_DrawText(buffer, xPosition, yPosition);
}
//!Only used for testing!! remove on release!!
void ShowTestEnemiesKilled(void){
	CP_Settings_Fill(TETRIS_COLOR);
	CP_Settings_TextSize(GetCellSize()/5.f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	char buffer[25] = { 0 };
	sprintf_s(buffer, 25, "TEST ENEMIES KILLED %02d", GetEnemiesKilled());
	float xPosition = WINDOWLENGTH/2.f;
	float yPosition = GetCellSize();
	CP_Font_DrawText(buffer, xPosition, yPosition);
}

void gameLevelExit(void){
	UnsubscribeAllEvents();
	ClearHearts();
	FreeIconImages();
	FreeEnemyDisplayIcon();
}