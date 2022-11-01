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

#pragma region SELECT_POINTER_VARIABLES

// Visual pointer to select menu option.
CP_Image selectPointer;

// How much to scale up the pointer from a 2x3 pixel.
int pointerScale = 15;

// How much pixel to offset pointer from button.
int pointerOffset = 40;

// Timer for blinking pointer.
Timer pointerBlink = {
	.time = .2f,
	.elaspedTime = 0,
};

int alpha = 255;

#pragma endregion

#pragma region TRANSITION_VARIABLES

// Timer for transition to next scene.
Timer transitionTimer = {
	.time = .5,
	.elaspedTime = 0,
};

// Timer for blinking button.
Timer buttonBlink = {
	.time = .1f,
	.elaspedTime = 0,
};

// Cache menu button that player clicked on. To know which button to blink.
Button* buttonClicked = NULL;

#pragma endregion

//___________________________________________________________________
// TODO: For debugging, remove before release
void Jarrett(void);

#pragma region Forward Declarations

void InitializeButtons(void);
void IntializeSelectPointer(void);
void DrawSelectPointer(void);
void HandleCarouselButton(void);
void HandleMenuButtonClick(void);
void HandleTransition(Button* btn);

void StartGame(void);
void Settings(void);
void Credits(void);
void ExitGame(void);

#pragma endregion

void MainMenuInit(void) {
	title = CP_Image_Load("Assets/MenuTitle.png");

	// Populate buttons with positional, size and text values.
	InitializeButtons();

	IntializeSelectPointer();
}

void MainMenuUpdate(void) {
	CP_Graphics_ClearBackground(MENU_BLACK);
	// Drawn 25% from top.
	CP_Image_Draw(title, GetWindowWidth() / 2, GetWindowHeight() / 4, (float)CP_Image_GetWidth(title) * GetWidthScale(), (float)CP_Image_GetHeight(title) * GetHeightScale(), 255);

	DrawButtons();
	DrawSelectPointer();

	HandleCarouselButton();
	HandleMenuButtonClick();

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
	float xPos = GetWindowWidth() / 9;
	float yPos = GetWindowHeight() / 2;
	// Buttons have an offset of 11% of window height.
	float offSet = GetWindowHeight() / 9;

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
	.heigth = textData.textSize * GetHeightScale(),
	.width = 400 * GetWidthScale(),
	};

	textData.text = "START";
	// Intialize button in UI manager.
	// OnClick: Start game.
	InitializeButton(&startBtn, startBtnRect, graphicData, textData, StartGame);

	/*========================Settings Button============================*/
	Rect settingsBtnRect = {
	.x = xPos,
	.y = yPos + (offSet * GetHeightScale()),
	.heigth = textData.textSize * GetHeightScale(),
	.width = 700 * GetWidthScale(),
	};

	textData.text = "SETTINGS";
	// Intialize button in UI manager.
	// OnClick: Settings screen.
	InitializeButton(&settingBtn, settingsBtnRect, graphicData, textData, Settings);

	/*========================Credits Button============================*/
	Rect creditsBtnRect = {
	.x = xPos,
	.y = yPos + (offSet * 2 * GetHeightScale()),
	.heigth = textData.textSize * GetHeightScale(),
	.width = 600 * GetWidthScale(),
	};

	textData.text = "CREDITS";
	// Intialize button in UI manager.
	// OnClick: Credits screen.
	InitializeButton(&creditsBtn, creditsBtnRect, graphicData, textData, Credits);

	/*=========================Exit Button=============================*/
	Rect quitBtnRect = {
	.x = xPos,
	.y = yPos + (offSet * 3 * GetHeightScale()),
	.heigth = textData.textSize * GetHeightScale(),
	.width = 300 * GetWidthScale(),
	};

	textData.text = "EXIT";
	// Intialize button in UI manager.
	// OnClick: Exit game.
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

	// When player click on a button, pointer will not move from the clicked button.
	if (buttonClicked){
		CP_Image_Draw(selectPointer, buttonClicked->transform.cachedPos.x - (pointerOffset * GetWidthScale()), buttonClicked->transform.cachedPos.y + (pointerOffset * GetHeightScale()), 2 * pointerScale * GetWidthScale(), 3 * pointerScale * GetHeightScale(), 255);
		return;
	}

	Button* hoverBtn = GetButtonHover();

	// Stop if no button hovered.
	if (hoverBtn == NULL){
		return;
	}

	if (pointerBlink.elaspedTime < pointerBlink.time){
		// Tick timer.
		pointerBlink.elaspedTime += CP_System_GetDt();
	}
	else {
		// Toggle visibility of pointer.
		alpha = (alpha == 0) ? 255 : 0;
		// Reset timer.
		pointerBlink.elaspedTime = 0;
	}

	// Image scale has to be 2:3 for its X and Y because pixel data is 2,3.
	CP_Image_Draw(selectPointer, hoverBtn->transform.cachedPos.x - (pointerOffset * GetWidthScale()), hoverBtn->transform.cachedPos.y + (pointerOffset * GetHeightScale()), 2 * pointerScale * GetWidthScale(), 3 * pointerScale * GetHeightScale(), alpha);
}

void HandleCarouselButton(){
	// Don't move button if transitioning.
	if (buttonClicked){
		// Return early to prevent further checks.
		return;
	}

	Button* hoverBtn = GetButtonHover();

	// If player is not hovering on any button, reset last hovered button position.
	if (GetPrevBtnHovered() != NULL){
		GetPrevBtnHovered()->transform.x = GetPrevBtnHovered()->transform.cachedPos.x;
	}

	// If hovering over a button.
	if (hoverBtn != NULL){
		// Offset button that player is hovering to create carousel effect.
		hoverBtn->transform.x = hoverBtn->transform.cachedPos.x + 35;
	}
}

void HandleMenuButtonClick(){
	if (buttonClicked != NULL){
		// Blink button that the player clicked on and then go to next scene.
		HandleTransition(buttonClicked);
	}
	else {
		// Check if player clicked any button.
		buttonClicked = GetButtonClick();
	}
}


void HandleTransition(Button* btn){
	// No blink when exiting game.
	if (btn == &exitBtn){
		transitionTimer.elaspedTime = transitionTimer.time;
	}

	if (transitionTimer.elaspedTime < transitionTimer.time){
		// Tick timers.
		transitionTimer.elaspedTime += CP_System_GetDt();
		buttonBlink.elaspedTime += CP_System_GetDt();

		// Time to blink.
		if (buttonBlink.elaspedTime >= buttonBlink.time){
			// Toggle color of button to make it blink.
			btn->textData.color = (btn->textData.color.a == 0) ? MENU_WHITE : TRANSPERANT;
			// Reset.
			buttonBlink.elaspedTime = 0;
		}
		return;
	}
	btn->callBack();
}

void StartGame(void){
	CP_Engine_SetNextGameState(gameLevelInit, gameLevelUpdate, gameLevelExit);
}

void Settings(void){
	CP_Engine_SetNextGameState(SettingsInit, SettingsUpdate, SettingsExit);
}

void Credits(void){
	CP_Engine_SetNextGameState(CreditsInit, CreditsUpdate, CreditsExit);
}

void ExitGame(void) {
	CP_Engine_Terminate();
}

