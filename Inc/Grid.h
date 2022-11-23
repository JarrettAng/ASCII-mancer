/*!
@file	  Grid.h
@author	  Muhammad Farhan Bin Ahmad (b.muhammadfarhan@digipen.edu)
@date     21/11/2022
@brief    1.Drawing of the grid
*		  2.Calculate,set and send info about each in the cell
________________________________________________________________________________________________________*/

#pragma once

#include <cprocessing.h>

#define WINDOWHEIGHT CP_System_GetWindowHeight()
#define WINDOWLENGTH CP_System_GetWindowWidth()

#define	TOTAL_YGRID 7
#define TOTAL_XGRID 13

typedef struct SpaceInfo
{
	float x_pos;
	float y_pos;
}SpaceInfo;
typedef struct CurrentGridPos
{
	int x_Index;
	int y_Index;
	float x_CenterPos;
	float y_CenterPos;
}CurrentGridPos;

float GetCellSize(void);
float GetGridTopBuffer(void);
float GetGridPlayingArea(void);

float GridYToPosY(int index);
float GridXToPosX(int index);

int PosXToGridX(float pos);
int PosYToGridY(float pos);

void grid_init(void);
void grid_update(void);

//Check if given point is in playing area
_Bool IsInPlayingArea(float x,float y);
_Bool IsIndexInPlayingArea(int x, int y);

//For drawing the grid
void DrawLineGrid();
//Create the space for players to place their blocks and enemy to move in
void CreatePlayingSpace();