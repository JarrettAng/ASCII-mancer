#include "cprocessing.h"
#include "ColorTable.h"
#include "GameOver.h"
#include "Grid.h"
#include "MainMenu.h"
#include "UIManager.h"
#include "Hearts.h"

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
	.textSize = 50 * GetHeightScale(),
	// Text will be drawn on the X Y pos of btn rect (which is top left corner),
	// So we shift text to top left.
	.hAlign = CP_TEXT_ALIGN_H_LEFT,
	.vAlign = CP_TEXT_ALIGN_V_TOP,
	};

	/*========================Main Menu button============================*/
	Rect BackMainMenuBtnRect = {
	.x = xPos - 120,
	.y = yPos + 340,
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

void RenderGOText(TextOnly Rendered_Text) {
	CP_Settings_Fill(Rendered_Text.color); // Color of text
	CP_Settings_TextSize(Rendered_Text.font_size); // Size of text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE); // Origin of text is it's absolute center
	CP_Font_DrawText(Rendered_Text.words, Rendered_Text.xpos, Rendered_Text.ypos);
}

void InitializeAllText(void) {
	Game_Over_Title.color = MENU_RED;
	Game_Over_Title.font_size = CP_System_GetDisplayWidth() / 15.f;
	Game_Over_Title.words = "GAME OVER! :(";
	Game_Over_Title.xpos = CP_System_GetDisplayWidth() / 2.f;
	Game_Over_Title.ypos = CP_System_GetDisplayHeight() / 4.f;

	Enemies_Killed.words = "ZOMBOIYOS KILLED :";
	Enemies_Killed.font_size = CP_System_GetDisplayWidth() / 30.f;
	Enemies_Killed.color = MENU_WHITE;
	Enemies_Killed.xpos = CP_System_GetDisplayWidth() / 2.f;
	Enemies_Killed.ypos = CP_System_GetDisplayHeight() * 1.5f / 3.f;


	Turns_Made.words = "TURNS MADE :";
	Turns_Made.font_size = CP_System_GetDisplayWidth() / 30.f;
	Turns_Made.color = MENU_WHITE;
	Turns_Made.xpos = CP_System_GetDisplayWidth() / 2.f;
	Turns_Made.ypos = CP_System_GetDisplayHeight() * 1.3f / 2.f;
}

void RenderAllText(void) {
	RenderGOText(Game_Over_Title);
	RenderGOText(Enemies_Killed);
	RenderGOText(Turns_Made);
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
	// Create the texts
	InitializeAllText();
}

void GameOverUpdate(void) {
	CP_Graphics_ClearBackground(GAMEOVER_DARKGRAY);
	CP_Font_Set(main_font); // Uses main_font for all fonts

	// Render text
	RenderAllText();

	// Draw buttons
	DrawButtons();
	HandleButtonClick();

	// FOR DEBUGGING PURPOSES DELETE LATER
	if (CP_Input_KeyTriggered(KEY_Z) == 1) {
		CP_Engine_Terminate();
	}
}

void GameOverExit(void) {
	ClearInteractCache();
	FreeUI();
}
