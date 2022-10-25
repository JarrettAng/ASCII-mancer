#include <cprocessing.h>
#include "MainMenu.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "Grid.h"

CP_Image title;

Button startBtn;
Button settingBtn;
Button creditsBtn;
Button exitBtn;

// Visual pointer to select menu option.
CP_Image selectPointer;

void MainMenuInit(void) {
	title = CP_Image_Load("Assets/MenuTitle.png");

	// Populate buttons with positional, size and text values.
	InitializeButtons();

	IntializeSelectPointer();
}

void MainMenuUpdate(void) {
	CP_Graphics_ClearBackground(MENU_BLACK);

	CP_Image_Draw(title, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 4, (float)CP_Image_GetWidth(title), (float)CP_Image_GetHeight(title), 255);

	DrawButtons();
	CheckForButtonClick();
	DrawSelectPointer();
}

void MainMenuExit(void) {
	CP_Image_Free(&title);
	CP_Image_Free(&selectPointer);
	FreeButton();
}

void InitializeButtons(void) {
	// Starting position to draw the menu buttons.
	float xPos = (float)CP_System_GetWindowWidth() / 9;
	float yPos = (float)CP_System_GetWindowHeight() / 2;

	/*==============Button Graphics & Text Settings===================*/
	// Currently all buttons in menu are using this graphic & text setting

	GraphicData graphicData = {
	.color = TRANSPERANT,
	.strokeColor = TRANSPERANT,
	.strokeWeight = 0,
	// Draw from top left corner so menu button aligns.
	.imagePosMode = CP_POSITION_CORNER
	};

	TextData textData = {
	.color = MENU_WHITE,
	.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
	.textSize = 80,
	// Text will be drawn on the X Y pos of btn rect (which is top left corner),
	// So we shift text to top left.
	.hAlign = CP_TEXT_ALIGN_H_LEFT,
	.vAlign = CP_TEXT_ALIGN_V_TOP,
	};

	/*========================Start Button============================*/
	Rect startBtnRect = {
	.x = xPos,
	.y = yPos,
	.heigth = 80,
	.width = 400,
	};

	textData.text = "START";
	// Intialize button in UI manager.
	// Onclick: Start game.
	InitializeButton(&startBtn, startBtnRect, graphicData, textData, StartGame);

	/*========================Settings Button============================*/
	Rect settingsBtnRect = {
	.x = xPos,
	.y = yPos + 120,
	.heigth = 80,
	.width = 700,
	};

	textData.text = "SETTINGS";
	// Intialize button in UI manager.
	// Onclick: Settings screen.
	InitializeButton(&settingBtn, settingsBtnRect, graphicData, textData, Settings);

	/*========================Credits Button============================*/
	Rect creditsBtnRect = {
	.x = xPos,
	.y = yPos + 240,
	.heigth = 80,
	.width = 600,
	};

	textData.text = "CREDITS";
	// Intialize button in UI manager.
	// Onclick: Credits screen.
	InitializeButton(&creditsBtn, creditsBtnRect, graphicData, textData, Credits);

	/*=========================Exit Button=============================*/
	Rect quitBtnRect = {
	.x = xPos,
	.y = yPos + 360,
	.heigth = 80,
	.width = 300,
	};

	textData.text = "EXIT";
	// Intialize button in UI manager.
	// Onclick: Exit game.
	InitializeButton(&exitBtn, quitBtnRect, graphicData, textData, ExitGame);
}

void IntializeSelectPointer(){
	unsigned char menuPointerData[] = {
		MENU_RED_CODE,TRANSPERANT_CODE,
		MENU_RED_CODE,MENU_RED_CODE,
		MENU_RED_CODE,TRANSPERANT_CODE
	};

	selectPointer = CP_Image_CreateFromData(2, 3, menuPointerData);
}

void DrawSelectPointer(){

	Button* hoverBtn = NULL;

	hoverBtn = CheckForButtonHover();

	if (hoverBtn == NULL){
		return;
	}

	// Image scale has to be 2:3 for its X and Y because pixel data is 2,3.
	// TODO: CLEAN UP
	CP_Image_Draw(selectPointer, hoverBtn->transform.x - 40, hoverBtn->transform.y + 40, 30, 45, 255);
}

void StartGame(void) {
	CP_Engine_SetNextGameState(test_init, test_update, test_exit);
}

void Settings(void) {
}

void Credits(void) {
}

void ExitGame(void) {
	CP_Engine_Terminate();
}

