#include <cprocessing.h>
#include "MainMenu.h"
#include "Settings.h"
#include "Credits.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "Grid.h"
#include "gamelevel.h"
#include "testfile.h"
#include "Utils.h"

CP_Image title;

Button startBtn;
Button settingBtn;
Button creditsBtn;
Button exitBtn;

// Visual pointer to select menu option.
CP_Image selectPointer;

//___________________________________________________________________
// TODO: For debugging, remove before release
void Jarrett(void);

// Forward declarations
void InitializeButtons(void);
void IntializeSelectPointer(void);
void DrawSelectPointer(void);
void StartGame(void);
void Settings(void);
void Credits(void);
void ExitGame(void);

void MainMenuInit(void) {
	title = CP_Image_Load("Assets/MenuTitle.png");

	// Populate buttons with positional, size and text values.
	InitializeButtons();

	IntializeSelectPointer();
}

void MainMenuUpdate(void) {
	CP_Graphics_ClearBackground(MENU_BLACK);
	// Drawn 25% from top.
	CP_Image_Draw(title, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 4, (float)CP_Image_GetWidth(title) * GetWidthScale(), (float)CP_Image_GetHeight(title) * GetHeightScale(), 255);

	DrawButtons();
	HandleButtonClick();
	DrawSelectPointer();

	//___________________________________________________________________
	// TODO: For debugging, remove before release
	if (CP_Input_KeyTriggered(KEY_F10)) {
		Jarrett();
	}
}

void MainMenuExit(void) {
	CP_Image_Free(&title);
	CP_Image_Free(&selectPointer);
	FreeButton();
}

void InitializeButtons(void) {
	// Starting position to draw the menu buttons.
	// Drawn 11% from left.
	// Drawn 50% from top.
	float xPos = (float)CP_System_GetWindowWidth() / 9;
	float yPos = (float)CP_System_GetWindowHeight() / 2;
	//float offSet =

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
	.textSize = 80 * GetHeightScale(),
	// Text will be drawn on the X Y pos of btn rect (which is top left corner),
	// So we shift text to top left.
	.hAlign = CP_TEXT_ALIGN_H_LEFT,
	.vAlign = CP_TEXT_ALIGN_V_TOP,
	};

	/*========================Start Button============================*/
	Rect startBtnRect = {
	.x = xPos,
	.y = yPos,
	.heigth = 80 * GetHeightScale(),
	.width = 400 * GetWidthScale(),
	};

	textData.text = "START";
	// Intialize button in UI manager.
	// Onclick: Start game.
	InitializeButton(&startBtn, startBtnRect, graphicData, textData, StartGame);

	/*========================Settings Button============================*/
	Rect settingsBtnRect = {
	.x = xPos,
	.y = yPos + (120 * GetHeightScale()),
	.heigth = 80 * GetHeightScale(),
	.width = 700 * GetWidthScale(),
	};

	textData.text = "SETTINGS";
	// Intialize button in UI manager.
	// Onclick: Settings screen.
	InitializeButton(&settingBtn, settingsBtnRect, graphicData, textData, Settings);

	/*========================Credits Button============================*/
	Rect creditsBtnRect = {
	.x = xPos,
	.y = yPos + (240 * GetHeightScale()),
	.heigth = 80 * GetHeightScale(),
	.width = 600 * GetWidthScale(),
	};

	textData.text = "CREDITS";
	// Intialize button in UI manager.
	// Onclick: Credits screen.
	InitializeButton(&creditsBtn, creditsBtnRect, graphicData, textData, Credits);

	/*=========================Exit Button=============================*/
	Rect quitBtnRect = {
	.x = xPos,
	.y = yPos + (360 * GetHeightScale()),
	.heigth = 80 * GetHeightScale(),
	.width = 300 * GetWidthScale(),
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

	hoverBtn = GetButtonHover();

	if (hoverBtn == NULL){
		return;
	}

	// Image scale has to be 2:3 for its X and Y because pixel data is 2,3.
	// TODO: CLEAN UP
	CP_Image_Draw(selectPointer, hoverBtn->transform.x - (40 * GetWidthScale()), hoverBtn->transform.y + (40 * GetHeightScale()), 30 * GetWidthScale(), 45 * GetHeightScale(), 255);
}

void StartGame(void) {
	CP_Engine_SetNextGameState(gameLevelInit, gameLevelUpdate, gameLevelExit);
}

void Settings(void) {
	CP_Engine_SetNextGameState(SettingsInit, SettingsUpdate, SettingsExit);
}

void Credits(void) {
	CP_Engine_SetNextGameState(CreditsInit, CreditsUpdate, CreditsExit);
}

void ExitGame(void) {
	CP_Engine_Terminate();
}

