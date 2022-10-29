#include <cprocessing.h>
#include "Utils.h"

float heightScale = WINDOW_HEIGHT;
float widthScale = WINDOW_WIDTH;

#pragma region USER_INTERFACE

/// <summary>
/// Update scaling multiplier for UI element.
/// Use this when changing window size.
/// </summary>
void UpdateUIScale(){
	widthScale = (float)CP_System_GetWindowWidth() / WINDOW_WIDTH;
	heightScale = (float)CP_System_GetWindowHeight() / WINDOW_HEIGHT;
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

