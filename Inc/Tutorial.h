/*!
@file	  Tutorial.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     17/11/2022
@brief    This header file contains structs holding information for rendering arrows and text, as well as 2 functions,

		  TutorialInit - Should be called on game level load, sets up the text, other UI elements, and event subscriptions.
		  TutorialUpdate - Renders all the tutorial UI elements like the arrows & text, should be called in an update loop.
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "CProcessing.h" // For CP_Vector & CP_Color

/*______________________________________________________________
@brief Contains information for rendering text, like colors, position, size.
*/
typedef struct {
	char* text;
	CP_Vector pos;
	CP_Color color;
	float size;

	CP_Vector box_bounds;
	CP_Vector box_size;
} TutText;


/*______________________________________________________________
@brief Contains information for rendering arrows, like line thickness, start & end position.
*/
typedef struct {
	CP_Vector pos_start;
	CP_Vector pos_end;
	CP_Color color;
	float thickness;
} TutArrow;

/*______________________________________________________________
@brief Should be called on game level load, sets up the text and other UI elements, 
	   and event subscription to player update and end.
*/
void TutorialInit(void);

/*______________________________________________________________
@brief Renders all the tutorial UI elements like the arrows & text, should be called in an update loop.
*/
void TutorialUpdate(void);
