/*!
@file	  splashscreen.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@course	  CSD1401 - Software Engineering Project 1
@section  Section B
@tutorial Splashscreen
@date     14/09/2022
@brief    This source file contains 3 main functions for SetNextGameState (init, update, exit)
		  In update, there are 4 functions called (process_input, update, render, math_pow) 
*//*______________________________________________________________________*/

#include "cprocessing.h"
#include "splashscreen.h"

// Control splashscreen timing with these 3 variables, how long to fade in, stay, and fade out
float fadeInTime, showTime, fadeOutTime;

// Digipen logo related variables
CP_Image logo;
CP_Color white, black;
float currentTime, endFadeInTime, startFadeOutTime, totalShowTime;
float logoX, logoY;
float logoWidth, logoHeight;
int logoAlpha;

// Cursor circle tracking related variables
float cursorX, cursorY, cursorDiameter;

// Initialize all variables that don't change during the course of the animation
void splash_screen_init(void) {

	/*--------------------------------------------*
	 * Change splashscreen animation timings here *
	 * -------------------------------------------*/
	fadeInTime = 1.25;
	showTime = 1; // How long the logo should stay after fade in and before fade out
	fadeOutTime = 0.75;

	// All these time variables control when fade in should stop, fade out should start, etc.
	currentTime = 0;
	endFadeInTime = fadeInTime + 0.1f;
	startFadeOutTime = fadeInTime + showTime;
	totalShowTime = fadeInTime + showTime + fadeOutTime + 0.1f;

	logo = CP_Image_Load("Assets/DigiPen_BLACK.png");

	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);

	logoX = (float)CP_System_GetDisplayWidth() / 2;
	logoY = (float)CP_System_GetDisplayHeight() / 2;

	logoWidth = (float)CP_Image_GetWidth(logo);
	logoHeight = (float)CP_Image_GetHeight(logo);

	// Size of the circle that follows the cursor
	cursorDiameter = 10;

	// I prefer the window to cover the whole screen instead of just the logo
	CP_System_SetWindowSize(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
}

// Every frame check for player input, update based on input, and refresh the screen
void splash_screen_update(void) {
	process_input();
	update();
	render();
}

// Stores the coordinates of the player's cursor and check for the quit command
void process_input(void) {
	cursorX = CP_Input_GetMouseX();
	cursorY = CP_Input_GetMouseY();

	// For convenience, pressing Q will terminate the program 
	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_Terminate();
	}
}

// Keeps track of time elapsed and resets the time after the animation ends to loop the splashscreen
void update(void) {
	currentTime += CP_System_GetDt();

	if (currentTime > totalShowTime) {
		currentTime = 0;
	}
}

// Draws the logo and circle following the cursor
void render(void) {
	// The alpha value of the Digipen logo, dependent on the current time in the animation
	if (currentTime <= endFadeInTime) {
		float alphaValue = CP_Math_LerpFloat(0, 255, math_pow(currentTime / fadeInTime, 10));
		logoAlpha = (int)alphaValue;
	}

	if (currentTime >= startFadeOutTime) {
		float alphaValue = CP_Math_LerpFloat(255, 0, math_pow((currentTime - startFadeOutTime) / fadeOutTime, 10));
		logoAlpha = (int)alphaValue;
	}

	CP_Graphics_ClearBackground(white);

	// Draws the logo
	CP_Image_Draw(logo, logoX, logoY, logoWidth, logoHeight, logoAlpha);

	// Draws the circle following the cursor
	CP_Settings_Fill(black);
	CP_Graphics_DrawCircle(cursorX, cursorY, cursorDiameter);
}

// Multiplying the lerp by an exponent of 10 is used to make the logo fade in slow at the start and faster at the end
float math_pow(float base, int exponent) {
	int count = 0;
	float result = 1;
	while (count < exponent) {
		count++;
		result *= base;
	}

	return result;
}

void splash_screen_exit(void) {
	CP_Image_Free(&logo);
}
