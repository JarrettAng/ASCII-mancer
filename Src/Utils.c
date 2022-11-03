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

/// <summary>
/// Check if given position is within a certain boundary area.
/// Returns true/false.
/// </summary>
_Bool pointWithinArea(float areaX, float areaY, float areaWidth, float areaHeigth, float xPos, float yPos, CP_POSITION_MODE areaMode)
{
	// If image is drawn from the corner, click detection will be different from image drawn from center.
	if (areaMode == CP_POSITION_CORNER) {
		if ((xPos < areaX + areaWidth) && (xPos > areaX) && (yPos < areaY + areaHeigth) && (yPos > areaY)) {
			return TRUE;
		}
		return FALSE;
	}

	float halfWidth = areaWidth / 2;
	float halfHeigth = areaHeigth / 2;

	// Image drawn from center.
	if (xPos > areaX - halfWidth && xPos < areaX + halfWidth && yPos < areaY + halfHeigth && yPos > areaY - halfHeigth) {
		return TRUE;
	}
	return FALSE;
}

_Bool pointWithinCircle(float xPos, float yPos, float circleX, float circleY, float radius){
	if ((xPos < circleX + radius) && (xPos > circleX - radius) && (yPos < circleY + radius) && (yPos > circleY - radius)){
		return TRUE;
	}
	return FALSE;
}

