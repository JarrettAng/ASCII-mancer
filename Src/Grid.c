#include <cprocessing.h>
#include "math.h"

#define BLACK CP_Color_Create(0, 0, 0, 255)
#define GREEN CP_Color_Create(0, 255, 0, 255)

#define WINDOWHEIGHT 1080
#define WINDOWLENGTH 1920
#define	TOTAL_Y_GRID 9
#define TOTAL_X_GRID 17
struct SpaceInfo
{
	float x_pos;
	float y_pos;
};

struct SpaceInfo space[TOTAL_X_GRID][TOTAL_Y_GRID];


void CreateGrid();
void CreateSpace();

int cube_Length;
int cube_Height;
//Start at 0 so remember is value + 1
int total_Y_grid = TOTAL_Y_GRID;
int total_X_grid = TOTAL_X_GRID;
/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>
int a = 0;
/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>


void test_init(void)
{
	CP_System_SetWindowSize(WINDOWLENGTH, WINDOWHEIGHT);
	cube_Length = cube_Height = 100;
	CreateSpace();
}
void CreateSpace()
{
	for (int y = 0; y < total_Y_grid-1; y++)
	{
		for (int x = 0; x < total_X_grid; x++)
		{
			space[x][y].x_pos = (float)(40.f + (x * cube_Length));
			space[x][y].y_pos = (float)(160.f + (y * cube_Height));
			CP_Font_DrawText("Z", space[x][y].x_pos, space[x][y].y_pos);
		}
	}
}
void CreateGrid()
{
	CP_Graphics_ClearBackground(BLACK);
	CP_Settings_Stroke(GREEN);


	//9x17 grid
	//horizontal line
	for (float y = 0; y <= total_Y_grid; y++)
	{
		CP_Graphics_DrawLine(0.f, (float)y * cube_Height, 1920.f, (float)y * cube_Height);
	}
	//vertical line
	for (float x = 0; x <= total_X_grid; x++)
	{
		//offset y1 by 100
		CP_Graphics_DrawLine((float)x * cube_Length, 100.f, (float)x * cube_Length, 900.f);
	}
}
void test_update(void)
{
	CreateGrid();
	CreateSpace();
	CP_Settings_TextSize(50.f);
	if (CP_Input_MouseClicked())
	{
		a++;
	}
}
void test_exit(void)
{

}