/*!
@file	  Tutorial.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     14/11/2022
@brief    This header file
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "CProcessing.h"

typedef struct {
	char* text;
	CP_Vector pos;
	CP_Color color;
	float size;

	CP_Vector box_bounds;
	CP_Vector box_size;
} TutText;

typedef struct {
	CP_Vector pos_start;
	CP_Vector pos_end;
	CP_Color color;
	float thickness;
} TutArrow;

/*______________________________________________________________
@brief Should be called on game level load, sets up the text and other UI
	   elements, and event subscription to player update and end.
*/
void TutorialInit(void);

void TutorialUpdate(void);
