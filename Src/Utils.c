/*!
@file	  Utils.c
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This source file contains utilities function that is used in the project.
________________________________________________________________________________________________________*/
#include <cprocessing.h>
#include "Utils.h"

float widthScale = BASE_WINDOW_WIDTH;
float heightScale = BASE_WINDOW_HEIGHT;

#pragma region USER_INTERFACE
// Update scaling multiplier for UI element.
// Use this when changing window size.
void UpdateUIScale(){
	widthScale = GetWindowWidth() / BASE_WINDOW_WIDTH;
	heightScale = GetWindowHeight() / BASE_WINDOW_HEIGHT;
}

// Returns the current width of the screen.
float GetDisplayWidth(){
	return (float)CP_System_GetDisplayWidth();
}

// Returns the current height of the screen.
float GetDisplayHeight(){
	return (float)CP_System_GetDisplayHeight();
}

// Returns the current width of the window.
float GetWindowWidth()
{
	return (float)CP_System_GetWindowWidth();
}

// Returns the current height of the window.
float GetWindowHeight()
{
	return (float)CP_System_GetWindowHeight();
}

// Get height scaling for UI elements.
float GetHeightScale()
{
	return heightScale;
}

// Get width scaling for UI elements.
float GetWidthScale()
{
	return widthScale;
}
#pragma endregion

// Check if given position is within a certain boundary area.
// Returns True/False.
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

// Check if given position is within a certain boundary circle area.
// Returns True/False.
_Bool pointWithinCircle(float xPos, float yPos, float circleX, float circleY, float radius){
	if ((xPos < circleX + radius) && (xPos > circleX - radius) && (yPos < circleY + radius) && (yPos > circleY - radius)){
		return TRUE;
	}
	return FALSE;
}

