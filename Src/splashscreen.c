/*!
@file	  SplashScreen.c
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This source file for displaying the splash screen screen.
________________________________________________________________________________________________________*/

#include "cprocessing.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "ColorTable.h"
#include "Utils.h"
#include "EnemyStats.h"
#include "SoundManager.h"
#include "UIManager.h"

const float FADE_IN_DURATION = 3.0f;

float fade = 0;	// Current fade timer
CP_Image logo;	// Digipen splash screen logo
Text copyRight;

#pragma region FORWARD_DECLARATION
void InitTexts(void);
void FadeInLogo(void);
#pragma endregion

void SplashScreenInit() {
	// Intialize logo
	logo = CP_Image_Load("Assets/DigiPen_WHITE.png");

	CP_System_Fullscreen();
	// CP_System_SetWindowSize(1920, 1080);

	// Update the scale for the UI elements based on window size.
	UpdateUIScale();

	CP_System_SetFrameRate(60.f);

	CP_System_SetWindowTitle("ASCII-mancer");

	//Initialise static duration prefabs
	InitEnemyPool();		//Initialise enemies 
	InitSoundManager();		//Initialise sounds
	InitTexts();
}

void SplashScreenUpdate() {
	CP_Graphics_ClearBackground(MENU_BLACK);

	// Fade in the digipen logo
	FadeInLogo();
	// Draw copyright texts.
	RenderTextBoxes();
}

void SplashScreenExit() {
	// Free image and text.
	CP_Image_Free(&logo);
	FreeUI();
}

void InitTexts(){
	Rect copyrightRectData = {
		.x = 0,
		.y = GetWindowHeight() - GetWindowHeight() / 5,
		.width = GetWindowWidth()
	};

	TextData copyRightTextData = {
		.color = MENU_WHITE,
		.textSize = 20,
		.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
		.hAlign = CP_TEXT_ALIGN_H_CENTER,
		.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
		.text =
		"All content copyright 2022 Digipen Institute of Technology Singapore."
		"\n\n"
		"All Rights Reserved"
	};
	InitializeTextBox(&copyRight, copyrightRectData, copyRightTextData);
}

void FadeInLogo() {
	// Draw logo to be fade in.
	CP_Image_Draw(logo, (float)(CP_System_GetWindowWidth() / 2), (float)(CP_System_GetWindowHeight() / 2), (float)CP_Image_GetWidth(logo), (float)CP_Image_GetHeight(logo), (int)(fade * 255));
	// Fade in copyright text.
	copyRight.textData.color.a = (unsigned char)(fade * 255);

	// Increment timer.
	fade += CP_System_GetDt() / FADE_IN_DURATION;

	// Change state when finish fading in.
	if (fade > 1) {
		CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
	}
}


