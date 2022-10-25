#include "cprocessing.h"
#include "ColorTable.h"
#include "GameOver.h"
#include "Grid.h"
#include "MainMenu.h"
#include "UIManager.h"

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

void BackToMainMenu(void) {
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}

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
	.textSize = 50,
	// Text will be drawn on the X Y pos of btn rect (which is top left corner),
	// So we shift text to top left.
	.hAlign = CP_TEXT_ALIGN_H_LEFT,
	.vAlign = CP_TEXT_ALIGN_V_TOP,
	};

	/*========================Main Menu button============================*/
	Rect BackMainMenuBtnRect = {
	.x = xPos - 120,
	.y = yPos + 340,
	.heigth = 110,
	.width = 880,
	};

	GameOverTextData.text = "MAIN MENU";
	// Intialize button in UI manager.
	// Onclick: Start game.
	InitializeButton(&backtomainmenuBtn, BackMainMenuBtnRect, GameOverGraphicData, GameOverTextData, BackToMainMenu);
}

//===========================================
// Text and Button function definitions
//===========================================

TextOnly CreateText(char* words, float size) {
	TextOnly New_Text;
	New_Text.font_size = size;
	New_Text.color;
	New_Text.words = words;
	return New_Text;
}

void RenderText(TextOnly Rendered_Text, float pos_x, float pos_y, CP_Color color) {
	Rendered_Text.color = color;
	CP_Settings_Fill(Rendered_Text.color); // Color of text
	CP_Settings_TextSize(Rendered_Text.font_size); // Size of text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE); // Origin of text is it's absolute center
	CP_Font_DrawText(Rendered_Text.words, pos_x, pos_y);
}

//=========================================================
// 
// GAME OVER STATE DEFINITIONS
// 
//=========================================================

int EnemiesKilled[1] = { 7 };
int TurnsMade[1] = { 10 };

void GameOverInit(void) {
	CP_System_Fullscreen();
	CP_Graphics_ClearBackground(GAMEOVER_DARKGRAY);
	main_font = CP_Font_Load("Assets/PressStart2P-Regular.ttf");

	// Populate buttons with positional, size and text values
	InitializeButtonsGameOverScreen();
	Game_Over_Title = CreateText("GAME OVER! :(", HeaderFontSize);
	Enemies_Killed = CreateText("ZOMBOIYOS KILLED : 10", 50.f);
	Enemies_Killed_Number = CreateText((char)EnemiesKilled[1], 50.f);
	Turns_Made = CreateText("TURNS MADE : 7", 50.f);
	Turns_Made_Number = CreateText((char)TurnsMade[1], 50.f);
}

void GameOverUpdate(void) {
	CP_Graphics_ClearBackground(GAMEOVER_DARKGRAY);
	CP_Font_Set(main_font); // Uses main_font for all fonts

	// Create and Render Header //
	RenderText(Game_Over_Title, CP_System_GetDisplayWidth() / 2.f, CP_System_GetDisplayHeight() / 2.f - 300.f, MENU_RED);

	//Render Text//
	RenderText(Enemies_Killed, CP_System_GetDisplayWidth() / 2.f, CP_System_GetDisplayHeight() / 2.f - 50.f, MENU_WHITE);
	RenderText(Turns_Made, CP_System_GetDisplayWidth() / 2.f, CP_System_GetDisplayHeight() / 2.f + 150.f, MENU_WHITE);


	// Draw buttons
	DrawButtons();
	CheckForButtonClick();

	if (CP_Input_KeyTriggered(KEY_Z) == 1) {
		CP_Engine_Terminate();
	}
}

void GameOverExit(void) {
	FreeButton();
}
