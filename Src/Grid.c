#include <cprocessing.h>
#include "Grid.h"
#include "ColorTable.h"


float cube_Length;

float grid_OffsetValue = 150.f;

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


void test_init(void) {

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
			space[x][y].x_pos = (float)(grid_Top + (x* cube_Length));
			space[x][y].y_pos = (float)(grid_Top + (y* cube_Length+(cube_Length/2.f));
		}
	}
}
void DrawLineGrid()
{
	CP_Graphics_ClearBackground(BLACK);
	CP_Settings_Stroke(GREEN);
	float grid_CurrentIndex = 1;

	CP_Graphics_DrawLine(0.f, grid_Top, (float)WINDOWLENGTH, grid_Top);
	CP_Graphics_DrawLine(0.f, grid_Bottom, (float)WINDOWLENGTH, grid_Bottom);
	//Control Y grid
	while (grid_CurrentIndex < TOTAL_YGRID)
	{
		CP_Graphics_DrawLine(grid_OffsetValue, grid_Top + (grid_CurrentIndex * (float)cube_Length), (float)WINDOWLENGTH, grid_Top + (grid_CurrentIndex * (float)cube_Length));
		grid_CurrentIndex++;
	}
	//Control XGrid
	for (float x = 0; x < TOTAL_XGRID; x++)
	{
		CP_Graphics_DrawLine(grid_OffsetValue + (cube_Length * x), grid_Top, grid_OffsetValue + (cube_Length * x), grid_Bottom);
	}

}
void test_update(void)
{
	DrawLineGrid();
}
void test_exit(void)
{

}