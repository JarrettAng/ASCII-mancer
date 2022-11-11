/*!
@file	  EnemyDisplay.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     09/11/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>

#include "Grid.h" // For cell size
#include "WaveSystem.h" // For alive enemy check

#include "EnemyDisplay.h"

CP_Vector display[4];
float text_size;

char text_buffer[4];

/*______________________________________________________________
@brief Called by Gamelevel during its initialization, this function will load
	   the values needed for rendering and such.
*/
void EnemyDisplayInit(void) {
	// Size of text is 20% of cell size
	text_size = GetCellSize() * 0.2f;

	float offset = GetCellSize() * 0.5f - text_size;
	// Initialize the offset positions for the corners
	display[TOP_RIGHT] = CP_Vector_Set(offset, -offset);
	display[BOTTOM_RIGHT] = CP_Vector_Set(offset, offset);
	display[BOTTOM_LEFT] = CP_Vector_Set(offset, offset);
	display[TOP_LEFT] = CP_Vector_Set(offset, -offset);
}

/*______________________________________________________________
@brief Displays the enemy stats in the four corners of the cell
*/
void RenderEnemyDisplay(float pos_x, float pos_y, CP_Color color, int health, int wall_damage) {
	CP_Settings_Fill(color);
	CP_Settings_TextSize(text_size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	// Render health
	if (health > 0) {
		sprintf_s(text_buffer, _countof(text_buffer), "%d", health);
		CP_Font_DrawText(text_buffer, pos_x + display[HEALTH].x, pos_y + display[HEALTH].y);
	}

	// Render wall damage if any
	if (wall_damage > 0) {
		sprintf_s(text_buffer, _countof(text_buffer), "%d", wall_damage);
		CP_Font_DrawText(text_buffer, pos_x + display[DAMAGE].x, pos_y + display[DAMAGE].y);
	}
}

/*______________________________________________________________
@brief 
*/
void RenderEnemyMovement(float pos_x, float pos_y, CP_Color color, int movement) {
	if (!movement) return;

	int actual_movement = 0, grid_x = PosXToGridX(pos_x), grid_y = PosYToGridY(pos_y);
	for (; actual_movement < movement; ++actual_movement) {
		if (HasLiveEnemyInCell(grid_x - (actual_movement + 1), grid_y)) break;
	}

	if (!actual_movement) return;

	color.a = 150;
	CP_Settings_Fill(color);
	CP_Settings_TextSize(text_size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	int tile = 0;
	for (; tile < actual_movement; ++tile) {
		CP_Font_DrawText(TEXT_MOVEMENT, pos_x - GetCellSize() * (tile + 0.5f), pos_y);
	}
	CP_Settings_TextSize(text_size * 0.75f);
	CP_Font_DrawText(TEXT_MOVEMENT_END_TOP, pos_x - GetCellSize() * tile, pos_y - GetCellSize() * 0.25f);
	CP_Font_DrawText(TEXT_MOVEMENT_END_BOTTOM, pos_x - GetCellSize() * tile, pos_y + GetCellSize() * 0.25f);
}
