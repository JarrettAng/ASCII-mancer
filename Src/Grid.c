#include <cprocessing.h>
#include "Grid.h"
#include "ColorTable.h"


float cube_Length;

float gridXOffset = 0;

///Screen split to 3 segment
/// TOP - HEALTH AND WAVE AREA(UI)
/// MIDDLE - GRID AREA
/// BOTTOM - TETRIS BLOCK AREA(UI)
//Percentage base
float area_Top = 10.f;
float area_Bottom = 80.f;
//Area sizes
float grid_Top = 0;
float grid_Bottom = 0;
float grid_PlayArea = 0;
/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>
int xpos = TOTAL_XGRID - 1;
int ypos = TOTAL_YGRID - 1;
/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>
SpaceInfo space[TOTAL_XGRID][TOTAL_YGRID];

void grid_init(void) {
	gridXOffset = WINDOWLENGTH*0.1f; 	//10% of screen on left side reserved for player
	grid_Top = WINDOWHEIGHT * area_Top / 100.f;
	grid_Bottom = WINDOWHEIGHT * area_Bottom / 100.f;
	grid_PlayArea = grid_Bottom - grid_Top;
	cube_Length = grid_PlayArea / TOTAL_YGRID;
	CP_Settings_StrokeWeight(1);
	CreatePlayingSpace();
}
void CreatePlayingSpace() {
	for (int y = 0; y < TOTAL_YGRID; y++)
	{
		for (int x = 1; x< TOTAL_XGRID; x++)
		{
			space[x][y].x_pos = (float)(grid_Top + (x* cube_Length/2));
			space[x][y].y_pos = (float)(grid_Top + (y* cube_Length/2.f));//Get center pos of cell
		}
	}
}
CP_Vector GetPosFromGrid(int x, int y){
	return CP_Vector_Set(space[x][y].x_pos,space[x][y].y_pos);
}

//Formula to get position to index (without offset)
//int x = (int)((GridPos/CP_System_GetWindowWidth())*width);
float GridYToPosY(int index){
	float PositionWithoutOffset = (float)(index*(grid_PlayArea/TOTAL_YGRID));
	return PositionWithoutOffset+grid_Top+(cube_Length/2);
}
float GridXToPosX(int index){
	float PositionWithoutOffset = (float)(index*((CP_System_GetWindowWidth()-gridXOffset)/TOTAL_XGRID));
	return PositionWithoutOffset+gridXOffset+(cube_Length/2);
}
int PosXToGridX(float pos){
	int x = (pos-gridXOffset-(cube_Length/2)/(WINDOWLENGTH-gridXOffset))*TOTAL_XGRID;
	return x;
}
int PosYToGridY(float pos){
	int y = (pos-grid_Top-(cube_Length/2)/grid_PlayArea)*TOTAL_YGRID;
	return y;
}


void DrawLineGrid()
{
	CP_Graphics_ClearBackground(BLACK);
	CP_Settings_Stroke(GREEN);
	float grid_CurrentIndex = 0;

	CP_Graphics_DrawLine(0.f, grid_Top, (float)WINDOWLENGTH, grid_Top);
	CP_Graphics_DrawLine(0.f, grid_Bottom, (float)WINDOWLENGTH, grid_Bottom);
	//Control Y grid
	while (grid_CurrentIndex < TOTAL_YGRID)
	{
		CP_Graphics_DrawLine(gridXOffset, grid_Top + (grid_CurrentIndex * (float)cube_Length), (float)WINDOWLENGTH, grid_Top + (grid_CurrentIndex * (float)cube_Length));
		grid_CurrentIndex++;
	}
	//Control XGrid
	for (int x = 0; x < TOTAL_XGRID; x++)
	{
		CP_Graphics_DrawLine(gridXOffset + ((WINDOWLENGTH-gridXOffset)/TOTAL_XGRID) * x, grid_Top, gridXOffset + (((WINDOWLENGTH-gridXOffset)/TOTAL_XGRID) * x), grid_Bottom);
	}

}
void grid_update(void)
{
	DrawLineGrid();
}
void grid_exit(void)
{

}