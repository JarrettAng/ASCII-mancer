#include "cprocessing.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "ColorTable.h"

#include "grid.h"	// TODO: Use scene manager instead.

const float FADE_IN_DURATION = 3.0f;

float fade = 0;	// Current fade timer
CP_Image logo;	// Digipen splash screen logo

#pragma region SPLASH_SCREEN_STATE

void SplashScreenInit() {
	// Intialize logo
	logo = CP_Image_Load("Assets/DigiPen_RED.png");

	CP_System_Fullscreen();

	CP_Graphics_ClearBackground(WHITE);
}

void SplashScreenUpdate() {
	// Fade in the digipen logo
	FadeInLogo();
}

void SplashScreenExit() {
	CP_Image_Free(&logo);
}

void FadeInLogo() {
	// Draw logo to be fade in.
	CP_Image_Draw(logo, (float)(CP_System_GetWindowWidth() / 2), (float)(CP_System_GetWindowHeight() / 2), (float)CP_Image_GetWidth(logo), (float)CP_Image_GetHeight(logo), (int)(fade * 255));

	// Increment timer.
	fade += CP_System_GetDt() / FADE_IN_DURATION;

	// Change state when finish fading in.
	if (fade > 1) {
		CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
	}
}

#pragma endregion

