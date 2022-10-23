#include <cprocessing.h>
#include "Grid.h"
#include "ColorTable.h"


int cube_Length;
int cube_Height;
//Start at 0 so remember is value + 1
int total_Y_grid = TOTAL_YGRID;
int total_X_grid = TOTAL_XGRID;

float grid_OffsetValue = 200.f;

/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>
/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>


void test_init(void){
	CP_System_SetWindowSize(WINDOWLENGTH, WINDOWHEIGHT);
	cube_Length = cube_Height = 100;
	CreatePlayingSpace();
}
void CreatePlayingSpace(){
	for (int y = total_Y_grid; y >= 0; y--)
	{
		for (int x = total_X_grid; x > 0; x--)
		{
			//NEED TO MAKE IT MODULOR
			space[x][y].x_pos = (float)((x * cube_Length + grid_OffsetValue/2 + 40.f));
			space[x][y].y_pos = (float)((y * cube_Height + grid_OffsetValue) + 60.f);
			CP_Settings_TextSize(50.f);
			CP_Font_DrawText("Z", space[x][y].x_pos, space[x][y].y_pos);
		}
	}
}
void DrawLineGrid()
{
	CP_Graphics_ClearBackground(CP_Color_Create(BLACK));
	CP_Settings_Stroke(CP_Color_Create(GREEN));

	int grid_ToDraw = total_Y_grid / 2;
	int grid_CenterLine = grid_ToDraw + 1;
	
	//RMD:1 GRID = 100
	//RMB TO REMOVE UNWANTED LINE IF WISH TO CHAGE AMOUNT OF GRID IN SCENE
	float grid_CurrentIndex = 0;
	///Starting at the center of the screen, half of the grid will be drawn above while the other drawn below
	// --------- TopHalf
	// --------- 
	// --------- CenterLine
	// ---------
	// --------- BottomHalf
	///	
	//Control YGrid
	while (grid_CurrentIndex <= grid_CenterLine)
	{
		if (grid_CurrentIndex <= grid_CenterLine)
		{
			if (grid_CurrentIndex == grid_CenterLine)//Last line
			{
				CP_Graphics_DrawLine(0.f, (float)WINDOWHEIGHTCENTER - grid_CurrentIndex * (float)cube_Height, (float)WINDOWLENGTH, (float)WINDOWHEIGHTCENTER - grid_CurrentIndex * (float)cube_Height);
				CP_Graphics_DrawLine(0.f, (float)WINDOWHEIGHTCENTER + grid_CurrentIndex * (float)cube_Height, (float)WINDOWLENGTH, (float)WINDOWHEIGHTCENTER + grid_CurrentIndex * (float)cube_Height);
			}
			else
			{
				CP_Graphics_DrawLine(grid_OffsetValue, (float)WINDOWHEIGHTCENTER - grid_CurrentIndex * (float)cube_Height, (float)WINDOWLENGTH, (float)WINDOWHEIGHTCENTER - grid_CurrentIndex * (float)cube_Height);
				CP_Graphics_DrawLine(grid_OffsetValue, (float)WINDOWHEIGHTCENTER + grid_CurrentIndex * (float)cube_Height, (float)WINDOWLENGTH, (float)WINDOWHEIGHTCENTER + grid_CurrentIndex * (float)cube_Height);
			}
			grid_CurrentIndex++;
		}
	}
	//Control XGrid
	for (float x = 0; x <= total_X_grid-1; x++)
	{
		CP_Graphics_DrawLine((float)x * cube_Length + grid_OffsetValue, (float)WINDOWHEIGHTCENTER + grid_CenterLine * cube_Height, (float)x * cube_Length+ grid_OffsetValue, (float)WINDOWHEIGHTCENTER - grid_CenterLine * cube_Height);
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