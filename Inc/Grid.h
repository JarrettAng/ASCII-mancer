#pragma once

#include <cprocessing.h>

#define WINDOWHEIGHT CP_System_GetWindowHeight()
#define WINDOWLENGTH CP_System_GetWindowWidth()
//GRID VALUE MUST ALWYAS BE ODD
#define	TOTAL_YGRID 11
#define TOTAL_XGRID 22

typedef struct SpaceInfo
{
	float x_pos;
	float y_pos;
}SpaceInfo;

float GridYToPosY(int index);
float GridXToPosX(int index);

int PosXToGridX(float pos);
int PosYToGridY(float pos);

void grid_init(void);
void grid_update(void);
void grid_exit(void);
//For drawing the grid
void DrawLineGrid();
//Create the space for players to place their blocks and enemy to move in
void CreatePlayingSpace();