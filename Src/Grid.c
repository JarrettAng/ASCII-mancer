#include <cprocessing.h>
#include "Grid.h"
#include "ColorTable.h"
#include <stdio.h>
#include <stdlib.h>


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
int xpos = TOTAL_XGRID;
int ypos = TOTAL_YGRID;
int x_Index = 0;
int y_Index = 0;

/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>
SpaceInfo space[TOTAL_XGRID][TOTAL_YGRID];
CurrentGridPos grid_Info;

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
			space[x][y].y_pos = (float)(grid_Top + (y* cube_Length/2.f));
		}
	}
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
	int x = (pos-gridXOffset)/(WINDOWLENGTH-gridXOffset)*TOTAL_XGRID;
	return x;
}
int PosYToGridY(float pos){
	int y = ((pos-grid_Top)/grid_PlayArea)*TOTAL_YGRID;
	return y;
}
//Check if mouse is in playing area
_Bool InPlayingArea()
{
	//Return value if inside playing area
	if (CP_Input_GetMouseY() > grid_Top && CP_Input_GetMouseY() < grid_Bottom && CP_Input_GetMouseX() > gridXOffset)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
struct CurrentGridPos CurrentPos(int x,int y)
{
	struct CurrentGridPos CGP;
	CGP.x_Index = x;
	CGP.y_Index = y;
	CGP.x_CenterPos = GridXToPosX(x);
	CGP.y_CenterPos = GridYToPosY(y);
	return CGP;
}
void DrawLineGrid()
{
	//Line =/ Grid position
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
	CP_Settings_Fill(MENU_RED);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER,CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(25.f);
	char buffer[25] = {0};

	x_Index = PosXToGridX(CP_Input_GetMouseX());//Get x index of grid
	y_Index = PosYToGridY(CP_Input_GetMouseY());//Get y index of grid
	grid_Info = CurrentPos(x_Index, y_Index);//A struct that contains the x,y index and the grid centerpoint
	//FOR DEBUGGING
	//Remove if no longer need to use
	if (InPlayingArea() == TRUE)
	{
		sprintf_s(buffer,25,"%d, %d,%.0f,%.0f", grid_Info.x_Index, grid_Info.y_Index, grid_Info.x_CenterPos, grid_Info.y_CenterPos);
		CP_Font_DrawText("Z", grid_Info.x_CenterPos, grid_Info.y_CenterPos);
	}
	CP_Font_DrawText(buffer,CP_Input_GetMouseX(),CP_Input_GetMouseY());
}
void grid_exit(void)
{

}