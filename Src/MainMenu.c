#include <cprocessing.h>
#include "MainMenu.h"
#include "SplashScreen.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "Grid.h"

CP_Image title;

Button startBtn;
Button settingBtn;
Button creditsBtn;
Button exitBtn;


void MainMenuInit(void) {
	title = CP_Image_Load("Assets/MenuTitle.png");

	// Populate buttons with positional, size and text values.
	InitializeButtons();
}

void MainMenuUpdate(void) {
	CP_Graphics_ClearBackground(CP_Color_Create(MENU_BLACK));

	CP_Image_Draw(title, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 4, CP_Image_GetWidth(title), CP_Image_GetHeight(title), 255);

	DrawButtons();
	CheckForButtonClick();
}

void MainMenuExit(void) {
	// Reset settings.
	CP_Image_Free(&title);
}

void InitializeButtons(void) {
	float xPos = (float)CP_System_GetWindowWidth() / 6;
	float yPos = (float)CP_System_GetWindowHeight() / 2;

	/*==============Button Graphics & Text Settings===================*/
	// Currently all buttons are using this graphic & text setting
	GraphicData graphicData = {
	.color = BLACK,
	.strokeWeigth = 0,
	.imagePosMode = CP_POSITION_CORNER
	};

	TextData textData = {
	.color = MENU_WHITE,
	.textSize = 100,
	.hAlign = CP_TEXT_ALIGN_H_LEFT,
	.vAlign = CP_TEXT_ALIGN_V_TOP
	};

	/*========================Start button============================*/
	Rect startBtnRect = {
	.x = xPos,
	.y = yPos,
	.heigth = 110,
	.width = 220,
	};

	textData.text = "Start";
	InitializeButton(&startBtn, startBtnRect, graphicData, textData, StartGame);

	/*=========================Quit button=============================*/
	Rect quitBtnRect = {
	.x = xPos,
	.y = yPos + 200,
	.heigth = 110,
	.width = 220,
	};

	textData.text = "Quit";
	InitializeButton(&exitBtn, quitBtnRect, graphicData, textData, ExitGame);
}

void StartGame(void) {
	CP_Engine_SetNextGameState(test_init, test_update, test_exit);
}

void ExitGame(void) {
	CP_Engine_Terminate();
}

