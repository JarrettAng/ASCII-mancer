#include <cprocessing.h>
#include "Utils.h"

float widthScale = BASE_WINDOW_WIDTH;
float heightScale = BASE_WINDOW_HEIGHT;

#pragma region USER_INTERFACE

/// <summary>
/// Update scaling multiplier for UI element.
/// Use this when changing window size.
/// </summary>
void UpdateUIScale(){
	widthScale = (float)CP_System_GetDisplayWidth() / BASE_WINDOW_WIDTH;
	heightScale = (float)CP_System_GetDisplayHeight() / BASE_WINDOW_HEIGHT;
	//widthScale = GetWindowWidth() / BASE_WINDOW_WIDTH;
	//heightScale = GetWindowHeight() / BASE_WINDOW_HEIGHT;
}

float GetWindowWidth()
{
	return (float)CP_System_GetWindowWidth();
}

float GetWindowHeight()
{
	return (float)CP_System_GetWindowHeight();
}


float GetHeightScale()
{
	return heightScale;
}

float GetWidthScale()
{
	return widthScale;
}

#pragma endregion

