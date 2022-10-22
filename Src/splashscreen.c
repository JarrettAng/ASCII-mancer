#include "cprocessing.h"
#include "SplashScreen.h"

const float FADE_IN_DURATION = 2.0f;

float fade = 0;	// Current fade timer
CP_Image logo;	// Digipen splash screen logo

#pragma region SPLASH_SCREEN_STATE

void Splash_screen_init() {
	// Intialize logo
	logo = CP_Image_Load("Assets/DigiPen_BLACK.png");
	// Set window size to fit logo
	CP_System_Fullscreen();
	// Initialize settings for circle so no need call every update frame
	CP_Settings_StrokeWeight(0);
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
}

void Splash_screen_update() {
	// Refresh screen
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

	// Fade in the digipen logo
	FadeInLogo();

	// Draw a circle that follows the mouse position
	DrawCircleFollowMouse();
}

void Splash_screen_exit() {
	CP_Image_Free(&logo);
}

void FadeInLogo() {
	// Draw logo to be fade in.
	CP_Image_Draw(logo, (float)(CP_System_GetWindowWidth() / 2), (float)(CP_System_GetWindowHeight() / 2), (float)CP_Image_GetWidth(logo), (float)CP_Image_GetHeight(logo), (int)(fade * 255));

	// Increment timer.
	fade += CP_System_GetDt() / FADE_IN_DURATION;

	// Reset fade
	if (fade > 1) {
		fade = 0;
	}
}

void DrawCircleFollowMouse() {
	// Create a red circle that follows the mouse.
	// Draw red circle above logo
	CP_Graphics_DrawCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 20.0f);
}

#pragma endregion

