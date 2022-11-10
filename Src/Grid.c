#include <cprocessing.h>
#include "Grid.h"
#include "ColorTable.h"
#include <stdio.h>
#include <stdlib.h>


float cube_Length = 0;

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
float grid_Left = 0;
float grid_Right = 0;
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
	grid_Top = WINDOWHEIGHT*0.1f;
	grid_Left = gridXOffset/2;
	grid_Right = WINDOWLENGTH-gridXOffset;

	//Calculations for adjusting buffers according to cell size
	float x = (WINDOWLENGTH*.8f)/TOTAL_XGRID;	//Width of playing space is 80% of screen width
	float y = (WINDOWHEIGHT*.625f)/TOTAL_YGRID;	//Height of playing space is 70% of screen height
	float size = min(x,y);						//Get whichever is smaller
	// if(WINDOWLENGTH-(size*TOTAL_XGRID)>(gridXOffset)){	//We prioritise adjusting by length
	// gridXOffset = (WINDOWLENGTH-(size*TOTAL_XGRID));		//Set the X Buffer first
	// grid_Top = (WINDOWHEIGHT-(size*TOTAL_YGRID))/4;		//have to adjust top accordingly (10% to top, 30% to bottom)
	// }
	if((WINDOWLENGTH-(size*TOTAL_XGRID))/2 > gridXOffset){
		gridXOffset =(WINDOWLENGTH-(size*TOTAL_XGRID))/2;
		grid_Top = (WINDOWHEIGHT-(size*TOTAL_YGRID))/2.75f;		//have to adjust top accordingly (10% to top, 30% to bottom)
	}

	//Regardless of anything adjustments, bottom is always playarea+grid_top
	grid_Bottom = ((size*TOTAL_YGRID)+grid_Top);

	grid_PlayArea = grid_Bottom - grid_Top;
	cube_Length = size;


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
//Returns the size of the grid cell
float GetCellSize(){
	return cube_Length;
}
float GetGridTopBuffer(){
	return grid_Top;
}
float GetGridPlayingArea(){
	return grid_PlayArea;
}
//Formula to get position to index (without offset)
//int x = (int)((GridPos/CP_System_GetWindowWidth())*width);
float GridYToPosY(int index){
	float PositionWithoutOffset = (float)(index*(grid_PlayArea/TOTAL_YGRID));
	return PositionWithoutOffset+grid_Top+(cube_Length/2);
}
float GridXToPosX(int index){
	float PositionWithoutOffset = (float)(index*((WINDOWLENGTH-(gridXOffset*2))/TOTAL_XGRID));
	return PositionWithoutOffset+gridXOffset+(cube_Length/2);
}

int PosXToGridX(float pos){
	int x = (pos-gridXOffset)/(WINDOWLENGTH-gridXOffset*2)*TOTAL_XGRID;
	return x;
}
int PosYToGridY(float pos){
	int y = ((pos-grid_Top)/grid_PlayArea)*TOTAL_YGRID;
	return y;
}

int GetGridCenterX(float pos){
	return GridXToPosX(PosXToGridX(pos));
}
int GetGridCenterY(float pos){
	return GridYToPosY(PosYToGridY(pos));
}
//Check if poiont is in playing area
_Bool IsInPlayingArea(float x,float y)
{	//GridX playing area starts from gridxOffset to the window width - (gridxoffset + 1 cube length)
	//GridY playing area starts from the grid top buffer to the grid bottom.
	return ((x > gridXOffset && x < WINDOWLENGTH-gridXOffset-(float)cube_Length)&&(y > grid_Top && y < grid_Bottom)) ? TRUE : FALSE;
}
_Bool IsIndexInPlayingArea(int x, int y){
	return (IsInPlayingArea(GridXToPosX(x), GridYToPosY(y)));
}
struct CurrentGridPos CurrentPos(int x,int y)
{
	struct CurrentGridPos CGP = {
		.x_Index = x,
		.y_Index = y,
		.x_CenterPos = GridXToPosX(x),
		.y_CenterPos = GridYToPosY(y),
	};
	return CGP;
}
void DrawLineGrid()
{
	//Line =/ Grid position
	CP_Graphics_ClearBackground(BLACK);
	CP_Settings_Stroke(BLACK);
	float grid_CurrentIndex = 0;

	CP_Graphics_DrawLine(0.f, grid_Top, (float)WINDOWLENGTH, grid_Top);
	CP_Graphics_DrawLine(0.f, grid_Bottom, (float)WINDOWLENGTH, grid_Bottom);
	//Control Y grid
	while (grid_CurrentIndex < TOTAL_YGRID)
	{
		CP_Graphics_DrawLine(gridXOffset, grid_Top + (grid_CurrentIndex * (float)cube_Length), (float)WINDOWLENGTH-gridXOffset-(float)cube_Length, grid_Top + (grid_CurrentIndex * (float)cube_Length));
		grid_CurrentIndex++;
	}
	//Control XGrid
	for (int x = 0; x < TOTAL_XGRID; x++)
	{
		CP_Graphics_DrawLine(gridXOffset + ((WINDOWLENGTH-gridXOffset*2)/TOTAL_XGRID) * x, grid_Top, gridXOffset + (((WINDOWLENGTH-gridXOffset*2)/TOTAL_XGRID) * x), grid_Bottom);
	}

}

void RenderGridCells(void) {
	for (short x = 0; x < TOTAL_XGRID - 1; ++x) {
		for (short y = 0; y < TOTAL_YGRID; ++y) {
			CP_Settings_Stroke(BLACK);
			CP_Settings_StrokeWeight(cube_Length * 0.1f);
			CP_Settings_Fill(GRID_COLOR);
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Graphics_DrawRect(GridXToPosX(x), GridYToPosY(y), cube_Length, cube_Length);
			CP_Settings_RectMode(CP_POSITION_CORNER);

			// Draw a dot in the cell if there is not zombie in it
			if (HasLiveEnemyInCell(x, y))continue;
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(MENU_WHITE);
			CP_Settings_StrokeWeight(cube_Length * 0.05f);
			CP_Graphics_DrawRect(GridXToPosX(x), GridYToPosY(y), cube_Length * 0.15f, cube_Length * 0.15f);
			CP_Settings_RectMode(CP_POSITION_CORNER);
		}
	}
}

void grid_update(void)
{
	DrawLineGrid();
	RenderGridCells();
	char buffer[25] = {0};

	x_Index = PosXToGridX(CP_Input_GetMouseX());//Get x index of grid
	y_Index = PosYToGridY(CP_Input_GetMouseY());//Get y index of grid
	grid_Info = CurrentPos(x_Index, y_Index);//A struct that contains the x,y index and the grid centerpoint
	//FOR DEBUGGING
	//Remove if no longer need to use
	// if (IsInPlayingArea(CP_Input_GetMouseX(),CP_Input_GetMouseY()))
	// {
	// 	sprintf_s(buffer,25,"%d, %d,%.0f,%.0f", grid_Info.x_Index, grid_Info.y_Index, grid_Info.x_CenterPos, grid_Info.y_CenterPos);
	// 	// CP_Font_DrawText("Z", grid_Info.x_CenterPos, grid_Info.y_CenterPos);
	// 	CP_Settings_Fill(MENU_RED);
	// 	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER,CP_TEXT_ALIGN_V_MIDDLE);
	// 	CP_Settings_TextSize(25.f);
	// 	CP_Font_DrawText("Z", GetGridCenterX(CP_Input_GetMouseX()),GetGridCenterY(CP_Input_GetMouseY()));

	// }
	// CP_Font_DrawText(buffer,CP_Input_GetMouseX(),CP_Input_GetMouseY());
}
void grid_exit(void)
{

}