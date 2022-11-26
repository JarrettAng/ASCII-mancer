/*!
@file	  GameOver.c
@author	  Justine Ilao (justine.c@digipen.edu)
@date     23/11/2022
@brief    This source file defines the GameOver states and all the function definitions to render all the
text in the Game Over screen
________________________________________________________________________________________________________*/

#include "cprocessing.h"
#include "ColorTable.h"
#include "GameOver.h"
#include "Grid.h"
#include "MainMenu.h"
#include "UIManager.h"
#include "Hearts.h"
#include "Win.h"
#include "WaveSystem.h"
#include <stdio.h>

#define ENEMIES_KILLED 12
#define TURNS_PASSED 7

CP_Font main_font;
Button backtomainmenuBtn;
CP_Image selectPointer;

TextOnly Game_Over_Title;
TextOnly Enemies_Killed;
TextOnly Enemies_Killed_Number;
TextOnly Turns_Made;
TextOnly Turns_Made_Number;

//============================================
// Button Settings
//============================================

/*----------------------------------------------------------------------------
@brief Sets the next game state to the main menu
------------------------------------------------------------------------------*/
void BackToMainMenu(void) {
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}

/*----------------------------------------------------------------------------
@brief Initializes the buttons like the back to main menu button in the game over
screen
------------------------------------------------------------------------------*/
void InitializeButtonsGameOverScreen(void) {
	float xPos = (float)CP_System_GetWindowWidth() / 6;
	float yPos = (float)CP_System_GetWindowHeight() / 2;

	/*==============Button Graphics & Text Settings===================*/
	// Currently all buttons in menu are using this graphic & text setting

	GraphicData GameOverGraphicData = {
	.color = TRANSPERANT,
	.strokeColor = TRANSPERANT,
	.strokeWeight = 0,
	// Draw from top left corner so menu button aligns.
	.imagePosMode = CP_POSITION_CORNER
	};

	TextData GameOverTextData = {
	.color = MENU_WHITE,
	.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
	.textSize = 50 * GetHeightScale(),
	// Text will be drawn on the X Y pos of btn rect (which is top left corner),
	// So we shift text to top left.
	.hAlign = CP_TEXT_ALIGN_H_LEFT,
	.vAlign = CP_TEXT_ALIGN_V_TOP,
	};

	/*========================Main Menu button============================*/
	Rect BackMainMenuBtnRect = {
	.x = xPos - CP_System_GetWindowWidth() / 10,
	.y = yPos + CP_System_GetWindowHeight() / 3,
	.heigth = GameOverTextData.textSize,
	.width = 400 * GetWidthScale(),
	};

	GameOverTextData.text = "MAIN MENU";
	// Intialize button in UI manager.
	// Onclick: Start game.
	InitializeButton(&backtomainmenuBtn, BackMainMenuBtnRect, GameOverGraphicData, GameOverTextData, BackToMainMenu);
}

//===========================================
// Text and Button function definitions
//===========================================

/*
void CreateText(TextOnly New_Text ,char* words, float font_size, CP_Color color, float xpos, float ypos) {
	New_Text.font_size = font_size;
	New_Text.color = color;
	New_Text.words = words;
	New_Text.xpos = xpos;
	New_Text.xpos = ypos;
}
*/

/*----------------------------------------------------------------------------
@brief Renders only the text, using the data from TextOnly structs
@param The TextOnly struct to be rendered
------------------------------------------------------------------------------*/
void RenderGOText(TextOnly Rendered_Text) {
	CP_Settings_Fill(Rendered_Text.color); // Color of text
	CP_Settings_TextSize(Rendered_Text.font_size); // Size of text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE); // Origin of text is it's absolute center
	CP_Font_DrawText(Rendered_Text.words, Rendered_Text.xpos, Rendered_Text.ypos);
}

/*----------------------------------------------------------------------------
@brief Initializes all the TextOnly structs
------------------------------------------------------------------------------*/
void InitializeAllText(void) {
	Game_Over_Title.color = MENU_RED;
	Game_Over_Title.font_size = CP_System_GetWindowWidth() / 15.f;
	Game_Over_Title.words = "GAME OVER! :(";
	Game_Over_Title.xpos = CP_System_GetWindowWidth() / 2.f;
	Game_Over_Title.ypos = CP_System_GetWindowHeight() / 4.f;

	Enemies_Killed.words = "ZOMBOIYOS KILLED :";
	Enemies_Killed.font_size = CP_System_GetWindowWidth() / 30.f;
	Enemies_Killed.color = MENU_WHITE;
	Enemies_Killed.xpos = CP_System_GetWindowWidth() / 2.f;
	Enemies_Killed.ypos = CP_System_GetWindowHeight() * 1.5f / 3.f;


	Turns_Made.words = "TURNS MADE :";
	Turns_Made.font_size = CP_System_GetWindowWidth() / 30.f;
	Turns_Made.color = MENU_WHITE;
	Turns_Made.xpos = CP_System_GetWindowWidth() / 2.f;
	Turns_Made.ypos = CP_System_GetWindowHeight() * 1.3f / 2.f;
}

/*----------------------------------------------------------------------------
@brief Renders all the TextOnly structs and as well as the Wave number and Enemies
Killed number in the Game Over screen
------------------------------------------------------------------------------*/
void RenderAllText(void) {
	RenderGOText(Game_Over_Title);

	char wave[25] = { 0 };
	sprintf_s(wave, 25, "WAVES SURVIVED : %02d/%02d", GetCurrentWave(), WAVES_TO_WIN);
	Turns_Made.words = wave;
	RenderGOText(Turns_Made);

	char killed[25] = { 0 };
	sprintf_s(killed, 25, "ZOMBOIYOS KILLED : %02d", GetEnemiesKilled());
	Enemies_Killed.words = killed;
	RenderGOText(Enemies_Killed);
}

//=========================================================
// 
// GAME OVER STATE DEFINITIONS
// 
//=========================================================

/*----------------------------------------------------------------------------
@brief GameOverInit state, initializes the font, the background and all the text
and buttons for the Game Over screen
------------------------------------------------------------------------------*/
void GameOverInit(void) {
	CP_Graphics_ClearBackground(GAMEOVER_DARKGRAY);
	main_font = CP_Font_Load("Assets/PressStart2P-Regular.ttf");

	// Populate buttons with positional, size and text values
	InitializeButtonsGameOverScreen();
	// Create the texts
	InitializeAllText();
}

/*----------------------------------------------------------------------------
@brief GameOverUpdate state, all the buttons and text are rendered in this state
------------------------------------------------------------------------------*/
void GameOverUpdate(void) {
	CP_Graphics_ClearBackground(GAMEOVER_DARKGRAY);
	CP_Font_Set(main_font); // Uses main_font for all fonts

	// Render text
	RenderAllText();

	// Draw buttons
	RenderButtons();
	HandleButtonClick();

	// FOR DEBUGGING PURPOSES DELETE LATER
	if (CP_Input_KeyTriggered(KEY_Z) == 1) {
		CP_Engine_Terminate();
	}
}

/*----------------------------------------------------------------------------
@brief GameOverExit state, resets the game parameters, wave count and enemies killed
as well as clearing the UI elements in the GameOver state
------------------------------------------------------------------------------*/
void GameOverExit(void) {
	ResetGame();
	ClearInteractCache();
	FreeUI();
}
