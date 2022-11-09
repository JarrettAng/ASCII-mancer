/*!
@file	  EnemyDisplay.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     09/11/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>

#include "Grid.h" // For cell size

#include "EnemyDisplay.h"

CP_Vector display[4];
float text_size;

char text_buffer[4];

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

void RenderEnemyDisplay(float pos_x, float pos_y, CP_Color color, int health, int wall_damage) {
	CP_Settings_Fill(color);
	CP_Settings_TextSize(text_size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	// Render health
	if (health >= 0) {
		sprintf_s(text_buffer, _countof(text_buffer), "%d", health);
		CP_Font_DrawText(text_buffer, pos_x + display[HEALTH].x, pos_y + display[HEALTH].y);
	}

	// Render wall damage if any
	if (wall_damage >= 0) {
		sprintf_s(text_buffer, _countof(text_buffer), "%d", wall_damage);
		CP_Font_DrawText(text_buffer, pos_x + display[DAMAGE].x, pos_y + display[DAMAGE].y);
	}
}
