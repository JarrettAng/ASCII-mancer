/*!
@file	  gamelevel.c
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@author	  Justine Carlo Villa Ilao (justine.c@digipen.edu)
@author	  Muhammad Farhan Bin Ahmad (b.muhammadfarhan@digipen.edu)
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This source file handles the rendering and the gameplay of the game. Essentially the main game scene.

@license  Copyright © 2022 DigiPen, All rights reserved.
________________________________________________________________________________________________________*/

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

#include "MainMenu.h" // For exiting to main menu.

_Bool debugMode = FALSE;


void gameLevelInit(void){
	// Zero out all the events in the event system first!
	ZeroOutAllEvents();
	//Start the game BGM
	PlayBGM(GAMEBGM);

	//INITIALISE GAME 
	grid_init();
	InitWaveSystem();
	InitializeLife();

	//INITIALISE UI ELEMENTS
	InitWizard();
	WinTextInit();
	InitMouseIcon();

	//INITIALISE TETRIS PIECE
	TManagerInit(DEFAULT_SPAWN_SEED);
	TPlayerInit();

	//INITIALISE TUTORIAL
	EnemyDisplayInit();
	TutorialInit();

	//Initialize game loop last, after all the events have been subscribed
	GameLoopInit();
}

void gameLevelUpdate(void){
	UpdateCameraShaker();

	//DRAWING AND UPDATING OF GRID
	grid_update();
	RenderEnemy();

	// Call the functions subscribed to the update events
	GameLoopUpdate();

	//UPDATE VFX
	UpdateEffects();
	RenderHand();
	ShowCurrentWave();
	//ShowTestEnemiesKilled();
	// UPDATE ROTATION ICON
	RenderMouseIcon();
	// Show enemy info on hover
	DisplayEnemyInfo();

	// UPDATE WIZARD
	UpdateWizard();
	// UPDATE HEARTS
	UpdateLife();
	// CHECK IF WON
	CheckWinCondition();
	// CLEAR SCREEN
	CP_Graphics_ClearBackground(BLACK);

	if (CP_Input_KeyTriggered(KEY_ESCAPE)){
		ExitToMainMenu();
	}



	//DEBUG MODE STUFF
	if(CP_Input_KeyDown(KEY_LEFT_SHIFT))
	{
		if(CP_Input_KeyTriggered(KEY_D)){
			debugMode = !debugMode;
		}
	}

	if(debugMode){
		ShowPlaytestMode();
		//SPAWN WALL
		if(CP_Input_KeyTriggered(KEY_0)){
	 		SpawnEnemyInCell(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()), GetEnemyPrefab(0));
		}
		//SPAWN NORMAL ZOMBIE
		if(CP_Input_KeyTriggered(KEY_1)){
	 		SpawnEnemyInCell(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()), GetEnemyPrefab(1));
		}
		//SPAWN LEAPER
		if(CP_Input_KeyTriggered(KEY_2)){
	 		SpawnEnemyInCell(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()), GetEnemyPrefab(2));
		}
		//SPAWN TANK
		if(CP_Input_KeyTriggered(KEY_3)){
	 		SpawnEnemyInCell(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()), GetEnemyPrefab(3));
		}
		//SPAWN BREAKER
		if(CP_Input_KeyTriggered(KEY_4)){
	 		SpawnEnemyInCell(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()), GetEnemyPrefab(4));
		}
		//SPAWN GRAVE
		if(CP_Input_KeyTriggered(KEY_5)){
	 		SpawnEnemyInCell(PosXToGridX(CP_Input_GetMouseX()), PosYToGridY(CP_Input_GetMouseY()), GetEnemyPrefab(5));
		}
		//CLEAR BOARD
		if(CP_Input_KeyTriggered(KEY_R)){
			ResetGame();
			ResetCameraShake();
		}
		//INSTANT WIN
		if(CP_Input_KeyTriggered(KEY_W)){
			SetWave(30);
		}
		//GAIN LIFE AND LOSE LIFE
		if (CP_Input_KeyReleased(KEY_O) == 1) {
			LoseLife(1);
		}
		if (CP_Input_KeyReleased(KEY_P) == 1) {
			GainLife(1);
		}
	}
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
	CP_Settings_TextSize(GetCellSize() / 5.f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	char buffer[25] = { 0 };
	sprintf_s(buffer, 25, "TEST ENEMIES KILLED %02d", GetEnemiesKilled());
	float xPosition = WINDOWLENGTH / 2.f;
	float yPosition = GetCellSize();
	CP_Font_DrawText(buffer, xPosition, yPosition);
}

void ShowPlaytestMode(void){
	CP_Settings_Fill(TETRIS_COLOR);
	CP_Settings_TextSize(GetCellSize() / 5.f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	char buffer[25] = { 0 };
	sprintf_s(buffer, 25, "PLAYTESTMODE");
	float xPosition = 0;
	float yPosition = GetCellSize()/5.f;
	CP_Font_DrawText(buffer, xPosition, yPosition);
}

void gameLevelExit(void){
	UnsubscribeAllEvents();
	ClearHearts();
	FreeIconImages();
	FreeMouseIcons();
	FreeEnemyDisplayIcon();
	ResetCameraShake();
}

void ExitToMainMenu(void){
	ResetGame();
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}