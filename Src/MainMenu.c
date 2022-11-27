/*!
@file	  MainMenu.c
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This source file for displaying the main menu screen.
________________________________________________________________________________________________________*/
#include <stdio.h>
#include <cprocessing.h>
#include "MainMenu.h"
#include "Options.h"
#include "Credits.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "Grid.h"
#include "gamelevel.h"
#include "Utils.h"
#include "SoundManager.h"

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
Button* transitionBtn = NULL;
// Timer for transition to next scene.
Timer transitionTimer = {
	.time = .5f,
	.elaspedTime = 0,
};

// Timer for blinking button.
Timer buttonBlink = {
	.time = .1f,
	.elaspedTime = 0,
};
#pragma endregion

#pragma region LOGO_SHINE_ANIMATION
// 13 Frame animation.
CP_Image logoAnim[13];
CP_Image logo;
CP_BOOL isShining = FALSE;

int frameLength = sizeof(logoAnim) / sizeof(logoAnim[0]);
int currentAnimFrame = 0;

Timer animIntervalTimer = {
	.time = 4,
	.elaspedTime = 4
};

Timer frameIntervalTimer = {
	.time = .075f,
	.elaspedTime = 0
};
#pragma endregion

#pragma region FORWARD_DECLARATIONS
void InitLogoAnim(void);
void InitMenuButtons(void);
void InitSelectPointer(void);

void DrawSelectPointer(void);

void HandleLogoAnim(void);
void HandleCarouselButton(void);
void HandleMenuButtonClick(void);
void HandleTransition(Button* btn);


void LoadGameScene(void);
void LoadOptionsScene(void);
void LoadCreditsScene(void);
void ExitGame(void);
#pragma endregion

void MainMenuInit(void) {
	// Initialize frames of logo animation.
	InitLogoAnim();
	logo = logoAnim[0];
	// Populate buttons with positional, size and text values.
	InitMenuButtons();
	// Create select pointer using pixel data.
	InitSelectPointer();

	PlayBGM(MAINMENU);
}

void MainMenuUpdate(void) {
	CP_Graphics_ClearBackground(MENU_BLACK);

	// Drawn 25% from top.
	CP_Image_Draw(logo, GetWindowWidth() / 2, GetWindowHeight() / 4, (float)CP_Image_GetWidth(logo) * GetWidthScale() * .8f, (float)CP_Image_GetHeight(logo) * GetHeightScale() * .8f, 255);

	// Render UI elements and handling UI interactions.
	UIManagerUpdate();
	// Draw select pointer on which button the player is hovering.
	DrawSelectPointer();
	// Apply carousel effect on button the player is hovering.
	HandleCarouselButton();
	// Handle scene transition.
	HandleMenuButtonClick();
	// Play logo shine animation.
	HandleLogoAnim();
}

void MainMenuExit(void) {
	CP_Image_Free(&logo);
	CP_Image_Free(*&logoAnim);
	CP_Image_Free(&selectPointer);
	transitionBtn = NULL;
	FreeUIManager();
}

void InitLogoAnim(){
	// Initalize animation per frame.
	// Buffer to store converted integer value.
	char str[3];
	// Starting file path.
	char filePath[] = "Assets/LogoAnim/Logo_00.png";
	// Loop through every frame.
	for (int i = 0; i < frameLength; ++i){
		// Converts int to char.
		sprintf_s(str, 3, "%02d", i);
		// Rename file path for current frame.
		filePath[21] = str[0];
		filePath[22] = str[1];
		// Cache file path for current frame in animation array.
		logoAnim[i] = CP_Image_Load(filePath);
	}
}

void HandleLogoAnim(){
	// Currently animating.
	if (isShining){
		// End of animation.
		if (currentAnimFrame >= frameLength){
			// Reset.
			currentAnimFrame = 0;
			isShining = !isShining;
			return;
		}

		// Interval between frames.
		if (frameIntervalTimer.elaspedTime >= frameIntervalTimer.time){
			// Update logo to anim frame image.
			frameIntervalTimer.elaspedTime = 0;
			logo = logoAnim[currentAnimFrame++];
		}
		else {
			// Tick timer.
			frameIntervalTimer.elaspedTime += CP_System_GetDt();
		}
		return;
	}
	// Interval between animation.
	if (animIntervalTimer.elaspedTime >= animIntervalTimer.time){
		// Start animation.
		animIntervalTimer.elaspedTime = 0;
		isShining = TRUE;
	}
	else {
		// Tick timer.
		animIntervalTimer.elaspedTime += CP_System_GetDt();
	}
}

void InitMenuButtons(void) {
	// Starting position to draw the menu buttons.
	// Drawn 11% from left.
	// Drawn 50% from top.
	float xPos = (float)BASE_WINDOW_WIDTH / 9 * GetWidthScale();
	float yPos = (float)BASE_WINDOW_HEIGHT / 2 * GetHeightScale();
	// Buttons have an offset of 11% of window height.
	float offSet = (float)BASE_WINDOW_HEIGHT / 9 * GetHeightScale();

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
	.heigth = textData.textSize,
	.width = 400 * GetWidthScale(),
	};

	textData.text = "START";
	// Intialize button in UI manager.
	// OnClick: Start game.
	InitializeButton(&startBtn, startBtnRect, graphicData, textData, LoadGameScene);

	/*========================Settings Button============================*/
	Rect settingsBtnRect = {
	.x = xPos,
	.y = yPos + offSet,
	.heigth = textData.textSize,
	.width = 600 * GetWidthScale(),
	};

	textData.text = "OPTIONS";
	// Intialize button in UI manager.
	// OnClick: Settings screen.
	InitializeButton(&settingBtn, settingsBtnRect, graphicData, textData, LoadOptionsScene);

	/*========================Credits Button============================*/
	Rect creditsBtnRect = {
	.x = xPos,
	.y = yPos + offSet * 2,
	.heigth = textData.textSize,
	.width = 600 * GetWidthScale(),
	};

	textData.text = "CREDITS";
	// Intialize button in UI manager.
	// OnClick: Credits screen.
	InitializeButton(&creditsBtn, creditsBtnRect, graphicData, textData, LoadCreditsScene);

	/*=========================Exit Button=============================*/
	Rect quitBtnRect = {
	.x = xPos,
	.y = yPos + offSet * 3,
	.heigth = textData.textSize,
	.width = 300 * GetWidthScale(),
	};

	textData.text = "QUIT";
	// Intialize button in UI manager.
	// OnClick: Exit game.
	InitializeButton(&exitBtn, quitBtnRect, graphicData, textData, ExitGame);
}

void InitSelectPointer(){
	unsigned char menuPointerData[] = {
		MENU_LOGO_RED_CODE,TRANSPERANT_CODE,
		MENU_LOGO_RED_CODE,MENU_LOGO_RED_CODE,
		MENU_LOGO_RED_CODE,TRANSPERANT_CODE
	};

	selectPointer = CP_Image_CreateFromData(2, 3, menuPointerData);
}

void DrawSelectPointer(){
	// When player click on a button, pointer will not move from the clicked button.
	if (transitionBtn != NULL){
		CP_Image_Draw(selectPointer, transitionBtn->transform.cachedPos.x - (pointerOffset * GetWidthScale()), transitionBtn->transform.cachedPos.y + (pointerOffset * GetHeightScale()), 2 * pointerScale * GetWidthScale(), 3 * pointerScale * GetHeightScale(), 255);
		return;
	}

	// Stop if no button hovered.
	if (GetBtnHovered() == NULL){
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
	CP_Image_Draw(selectPointer, GetBtnHovered()->transform.cachedPos.x - (pointerOffset * GetWidthScale()), GetBtnHovered()->transform.cachedPos.y + (pointerOffset * GetHeightScale()), 2 * pointerScale * GetWidthScale(), 3 * pointerScale * GetHeightScale(), alpha);
}

void HandleCarouselButton(){
	// Don't move button if transitioning.
	if (GetBtnClicked() != NULL){
		// Return early to prevent further checks.
		return;
	}

	// If player is not hovering on any button, reset last hovered button position.
	if (GetPrevBtnHovered() != NULL){
		GetPrevBtnHovered()->transform.x = GetPrevBtnHovered()->transform.cachedPos.x;
		GetPrevBtnHovered()->textData.color = MENU_WHITE;
	}

	// If hovering over a button.
	if (GetBtnHovered() != NULL){
		// Offset button that player is hovering to create carousel effect.
		GetBtnHovered()->transform.x = GetBtnHovered()->transform.cachedPos.x + 35;
		GetBtnHovered()->textData.color = LOGO_RED;
	}
}

void HandleMenuButtonClick(){
	// Cache the first button clicked.
	if (transitionBtn == NULL){
		transitionBtn = GetBtnClicked();
	}
	// Transition to the scene based on first button clicked.
	if (transitionBtn != NULL){
		// Blink button that the player clicked on and then go to next scene.
		HandleTransition(transitionBtn);
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
	// Reset.
	transitionTimer.elaspedTime = 0;
	btn->callBack();
}

// Transition to game scene.
void LoadGameScene(void){
	CP_Engine_SetNextGameState(gameLevelInit, gameLevelUpdate, gameLevelExit);
}

// Transition to option scene.
void LoadOptionsScene(void){
	CP_Engine_SetNextGameState(OptionsInit, OptionsUpdate, OptionsExit);
}

// Transition to credits scene.
void LoadCreditsScene(void){
	CP_Engine_SetNextGameState(CreditsInit, CreditsUpdate, CreditsExit);
}

// Exit game.
void ExitGame(void) {
	CP_Engine_Terminate();
}

