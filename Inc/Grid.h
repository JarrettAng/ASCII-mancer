#pragma once

#include <cprocessing.h>

#define WINDOWHEIGHT CP_System_GetWindowHeight()
#define WINDOWLENGTH CP_System_GetWindowWidth()
//GRID VALUE MUST ALWYAS BE EVEN
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
void grid_exit(void);

//Check if given point is in playing area
_Bool IsInPlayingArea(float x,float y);
_Bool IsIndexInPlayingArea(int x, int y);

//For drawing the grid
void DrawLineGrid();
//Create the space for players to place their blocks and enemy to move in
void CreatePlayingSpace();