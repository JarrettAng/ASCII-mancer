/*!
@file	  Utils.h
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This header file contains the declarations for Utils.c

@license  Copyright © 2022 DigiPen, All rights reserved.
________________________________________________________________________________________________________*/

#pragma once

// Base screen size.
// Aspect ratio: 16:9
#define BASE_WINDOW_WIDTH 1920
#define BASE_WINDOW_HEIGHT 1080

/*
@brief Update the scaling for UI element based on screen size.
*/
void UpdateUIScale(void);

/*
@brief Get the current window width.
*/
float GetWindowWidth(void);

/*
@brief Get the current window height.
*/
float GetWindowHeight(void);

/*
@brief Get the current display width.
*/
float GetDisplayWidth(void);

/*
@brief Get the current window width.
*/
float GetDisplayHeight(void);

/*
@brief Get the width scale for UI elements to scale by.
*/
float GetWidthScale(void);

/*
@brief Get the height scale for UI elements to scale by.
*/
float GetHeightScale(void);

typedef void(*Callback)(void);

/*
@brief Struct to cache variables for a simple timer.
*/
typedef struct Timer{
	float time;
	float elaspedTime;
} Timer;

/*
@brief Check if given point is within a given area. Returns True/False.
*/
_Bool pointWithinArea(float areaX, float areaY, float areaWidth, float areaHeigth, float xPos, float yPos, CP_POSITION_MODE areaMode);

/*
@brief Check if given point is within a given circle. Returns True/False.
*/
_Bool pointWithinCircle(float xPos, float yPos, float circleX, float circleY, float radius);
