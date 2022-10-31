#pragma once

#include <cprocessing.h>

#define WINDOWHEIGHT CP_System_GetWindowHeight()
#define WINDOWLENGTH CP_System_GetWindowWidth()
//GRID VALUE MUST ALWYAS BE ODD
#define	TOTAL_YGRID 11
#define TOTAL_XGRID 26

typedef struct SpaceInfo
{
	float x_pos;
	float y_pos;
}SpaceInfo;

struct SpaceInfo space[TOTAL_XGRID][TOTAL_YGRID];

void test_init(void);
void test_update(void);
void test_exit(void);
//For drawing the grid
void DrawLineGrid();
//Create the space for players to place their blocks and enemy to move in
void CreatePlayingSpace();