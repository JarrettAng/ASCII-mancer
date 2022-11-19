#include <cprocessing.h>
#include "Grid.h"
#include "ColorTable.h"
#include <stdio.h>
#include <stdlib.h>
#include "WaveSystem.h"

/*
* @Code brief:
*	1.Code that set up the playing area of the game
*	2.All value must be set to ensure the grid is in propotion to the current player size screen
*	3.Grid draw and the cell are 2 different entities
*	4.Grid uses a 2D array to set the cell
*/

float cellLength = 0;
float gridXOffset = 0;
///Screen split to 3 segment
/// TOP - HEALTH AND WAVE AREA(UI)
/// MIDDLE - GRID AREA
/// BOTTOM - TETRIS BLOCK AREA(UI)

//Area sizes
float gridTop = 0;
float gridBottom = 0;
float grid_PlayArea = 0;
/// <TEST VARIABLE(RMB TO REMOVE WHEN DONE)>
int xpos = TOTAL_XGRID;
int ypos = TOTAL_YGRID;
int x_Index = 0;
int y_Index = 0;

// Exclamation mark settings
float exclaim_elapsed_time;
float exclaim_blink_speed = 0.35f;

SpaceInfo space[TOTAL_XGRID][TOTAL_YGRID];
CurrentGridPos grid_Info;

/*______________________________________________________________
@brief Set the grid and cell for the playing at the start of the game
______________________________________________________________*/
void grid_init(void) {
	/*
	* Set offset value for drawing the grid
	* Black spaces are non playing area
	*/
	gridXOffset = WINDOWLENGTH*0.1f; 	
	gridTop = WINDOWHEIGHT*0.1f;	

	//Calculations for adjusting buffers according to cell size
	float x = (WINDOWLENGTH*.8f)/TOTAL_XGRID;	//Width of playing space is 80% of screen width
	float y = (WINDOWHEIGHT*.625f)/TOTAL_YGRID;	//Height of playing space is 70% of screen height
	float size = min(x,y);						//Get whichever is smaller

	if((WINDOWLENGTH-(size*TOTAL_XGRID))/2 > gridXOffset){
		gridXOffset =(WINDOWLENGTH-(size*TOTAL_XGRID))/2;
		gridTop = (WINDOWHEIGHT-(size*TOTAL_YGRID))/2.75f;		//have to adjust top accordingly (10% to top, 30% to bottom)
	}

	//Regardless of anything adjustments, bottom is always playarea+gridTop
	gridBottom = ((size*TOTAL_YGRID)+gridTop);

	//Set player area(with offset)
	grid_PlayArea = gridBottom - gridTop;
	//set cell size
	cellLength = size;

	// Initialize exclaimation mark time setting
	exclaim_elapsed_time = 0.0f;

	CP_Settings_StrokeWeight(1);
	CreatePlayingSpace();
}
/*______________________________________________________________
@brief Create the cell for player to place their piece and enemy to move in
______________________________________________________________*/
void CreatePlayingSpace() {
	for (int y = 0; y < TOTAL_YGRID; y++){
		for (int x = 1; x< TOTAL_XGRID; x++){
			space[x][y].x_pos = (float)(gridTop + (x* cellLength/2));//set index pos for x-pos
			space[x][y].y_pos = (float)(gridTop + (y* cellLength/2.f));//set index pos for y-pos
		}
	}
}
/*______________________________________________________________
@brief Returns the size of the grid cell
______________________________________________________________*/
float GetCellSize(){
	return cellLength;
}
float GetGridTopBuffer(){
	return gridTop;
}
float GetGridPlayingArea(){
	return grid_PlayArea;
}
/*______________________________________________________________
@brief Formula to get y position to index (without offset)
______________________________________________________________*/
float GridYToPosY(int index){
	float PositionWithoutOffset = (float)(index*(grid_PlayArea/TOTAL_YGRID));
	return PositionWithoutOffset+gridTop+(cellLength/2);
}
/*______________________________________________________________
@brief Formula to get x position to index (without offset)
______________________________________________________________*/
float GridXToPosX(int index){
	float PositionWithoutOffset = (float)(index*((WINDOWLENGTH-(gridXOffset*2))/TOTAL_XGRID));
	return PositionWithoutOffset+gridXOffset+(cellLength/2);
}
/*______________________________________________________________
@brief Grab current cell based on x-axis
______________________________________________________________*/
int PosXToGridX(float pos){
	int x = (int)((pos-gridXOffset)/(WINDOWLENGTH-gridXOffset*2)*TOTAL_XGRID);
	return x;
}
/*______________________________________________________________
@brief Grab current cell based on y-axis
______________________________________________________________*/
int PosYToGridY(float pos){
	int y = (int)(((pos-gridTop)/grid_PlayArea)*TOTAL_YGRID);
	return y;
}

/*______________________________________________________________
@brief Check if current mouse pos is in playing area
______________________________________________________________*/
_Bool IsInPlayingArea(float x,float y)
{	//GridX playing area starts from gridxOffset to the window width - (gridxoffset + 1 cube length)
	//GridY playing area starts from the grid top buffer to the grid bottom.
	return ((x > gridXOffset && x < WINDOWLENGTH-gridXOffset-(float)cellLength)&&(y > gridTop && y < gridBottom)) ? TRUE : FALSE;
}
_Bool IsIndexInPlayingArea(int x, int y){
	//Return index value of current x cell and y cell the player is in
	return (IsInPlayingArea(GridXToPosX(x), GridYToPosY(y)));
}
/*______________________________________________________________
@brief Get current position(x and y) and current cell index(x and y)
______________________________________________________________*/
struct CurrentGridPos CurrentPos(int x,int y){
	struct CurrentGridPos CGP = {
		.x_Index = x,
		.y_Index = y,
		.x_CenterPos = GridXToPosX(x),
		.y_CenterPos = GridYToPosY(y),
	};
	return CGP;
}
/*______________________________________________________________
@brief Draw grid line
______________________________________________________________*/
void DrawLineGrid(){
	CP_Settings_Stroke(BLACK);
	float grid_CurrentIndex = 0;
	/*
	* NOTE:
	* THIS ONLY DRAW A GRID. DOES NOT SET UP GRID CELL
	*/
	CP_Graphics_DrawLine(0.f, gridTop, (float)WINDOWLENGTH, gridTop);
	CP_Graphics_DrawLine(0.f, gridBottom, (float)WINDOWLENGTH, gridBottom);
	//Control Y grid
	while (grid_CurrentIndex < TOTAL_YGRID){
		CP_Graphics_DrawLine(gridXOffset, gridTop + (grid_CurrentIndex * (float)cellLength), (float)WINDOWLENGTH-gridXOffset-(float)cellLength, gridTop + (grid_CurrentIndex * (float)cellLength));
		grid_CurrentIndex++;
	}
	//Control XGrid
	for (int x = 0; x < TOTAL_XGRID; x++){
		CP_Graphics_DrawLine(gridXOffset + ((WINDOWLENGTH-gridXOffset*2)/TOTAL_XGRID) * x, gridTop, gridXOffset + (((WINDOWLENGTH-gridXOffset*2)/TOTAL_XGRID) * x), gridBottom);
	}

}
/*______________________________________________________________
@brief Render other(aesthetics) stuff thats inside the grid
______________________________________________________________*/
void RenderGridCells(void) {
	// Zombie near warning timer
	exclaim_elapsed_time += CP_System_GetDt();

	for (short y = 0; y < TOTAL_YGRID; ++y) {
		// Draw a exclamation mark on rows with enemies about to kill the player
		if ((GetAliveEnemyFromGrid(0, y) && GetAliveEnemyFromGrid(0, y)->type != WALL) || // Check the 3 columns closest to the player for enemies
			(GetAliveEnemyFromGrid(1, y) && GetAliveEnemyFromGrid(1, y)->type != WALL) ||
			(GetAliveEnemyFromGrid(2, y) && GetAliveEnemyFromGrid(2, y)->type != WALL)) {

			// If there any, draw a exclamation mark ! on the left end of the row
			CP_Settings_Fill(MENU_RED);
			CP_Settings_TextSize(cellLength * 0.85f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

			if (exclaim_elapsed_time > exclaim_blink_speed) {
				CP_Font_DrawText("!", GridXToPosX(-1), GridYToPosY(y));

				if (exclaim_elapsed_time > exclaim_blink_speed * 2) {
					exclaim_elapsed_time = 0.0f;
				}
			}
		}

		for (short x = 0; x < TOTAL_XGRID - 1; ++x) {
			//Draw grid line
			CP_Settings_Fill(GRID_COLOR);
			CP_Settings_Stroke(BLACK);
			CP_Settings_StrokeWeight(cellLength * 0.1f);
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Graphics_DrawRect(GridXToPosX(x), GridYToPosY(y), cellLength, cellLength);
			CP_Settings_RectMode(CP_POSITION_CORNER);

			// Draw a dot in the cell if there is not zombie in it
			if (HasLiveEnemyInCell(x, y))continue;
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(MENU_WHITE);
			CP_Settings_StrokeWeight(cellLength * 0.05f);
			CP_Graphics_DrawRect(GridXToPosX(x), GridYToPosY(y), cellLength * 0.15f, cellLength * 0.15f);
			CP_Settings_RectMode(CP_POSITION_CORNER);
		}
	}
}

void grid_update(void){
	DrawLineGrid();
	RenderGridCells();
	char buffer[25] = {0};

	x_Index = PosXToGridX(CP_Input_GetMouseX());//Get x index of grid
	y_Index = PosYToGridY(CP_Input_GetMouseY());//Get y index of grid
	grid_Info = CurrentPos(x_Index, y_Index);//A struct that contains the x,y index and the grid centerpoint
}
