#include <cprocessing.h>
#include "Grid.h"
#include "ColorTable.h"


int cube_Length;
//Start at 0 so remember is value + 1
int total_Y_grid = TOTAL_YGRID;
int total_X_grid = TOTAL_XGRID;

float grid_OffsetValue = 400.f;

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
/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>


void test_init(void) {

	grid_Top = WINDOWHEIGHT * area_Top / 100.f;
	grid_Bottom = WINDOWHEIGHT * area_Bottom / 100.f;
	grid_PlayArea = grid_Bottom - grid_Top;
	CP_Settings_StrokeWeight(1);
	cube_Length  = grid_PlayArea / TOTAL_YGRID;
	CreatePlayingSpace();
}
void CreatePlayingSpace() {
	for (int y = total_Y_grid; y >= 0; y--)
	{
		for (int x = total_X_grid; x > 0; x--)
		{
			//NEED TO MAKE IT MODULOR
			space[x][y].x_pos = (float)((x * cube_Length + grid_OffsetValue / 2 + 40.f));
			space[x][y].y_pos = (float)((y * cube_Length + grid_OffsetValue) + 60.f);
			//CP_Settings_TextSize(50.f);
			//CP_Font_DrawText("Z", space[x][y].x_pos, space[x][y].y_pos);
		}
	}
}
void DrawLineGrid()
{
	CP_Graphics_ClearBackground(BLACK);
	CP_Settings_Stroke(GREEN);

	int grid_ToDraw = total_Y_grid / 2;

	//RMD:1 GRID = 100
	//RMB TO REMOVE UNWANTED LINE IF WISH TO CHAGE AMOUNT OF GRID IN SCENE
	float grid_CurrentIndex = 1;
	///Starting at the center of the screen, half of the grid will be drawn above while the other drawn below
	// --------- TopHalf
	// --------- 
	// --------- CenterLine
	// ---------
	// --------- BottomHalf
	///	
	//Control YGrid
	CP_Graphics_DrawLine(0.f, grid_Top, (float)WINDOWLENGTH, grid_Top);
	CP_Graphics_DrawLine(0.f, grid_Bottom, (float)WINDOWLENGTH, grid_Bottom);
	while (grid_CurrentIndex < TOTAL_YGRID)
	{
		CP_Graphics_DrawLine(grid_OffsetValue, grid_Top + (grid_CurrentIndex * (float)cube_Length), (float)WINDOWLENGTH, grid_Top + (grid_CurrentIndex * (float)cube_Length));
		grid_CurrentIndex++;
	}
	//Control XGrid
	for (float x = 0; x < total_X_grid; x++)
	{
		CP_Graphics_DrawLine(grid_OffsetValue + (cube_Length * x), grid_Top, grid_OffsetValue + (cube_Length * x), grid_Bottom);
	}

}
void test_update(void)
{
	DrawLineGrid();
	CreatePlayingSpace();
}
void test_exit(void)
{

}