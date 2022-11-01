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
	widthScale = GetDisplayWidth() / BASE_WINDOW_WIDTH;
	heightScale = GetDisplayHeight() / BASE_WINDOW_HEIGHT;
}

float GetDisplayWidth(){
	return (float)CP_System_GetDisplayWidth();
}

float GetDisplayHeight(){
	return (float)CP_System_GetDisplayHeight();
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

