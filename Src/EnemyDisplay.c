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
#include "ColorTable.h"

#include "EnemyDisplay.h"

CP_Vector display[4];

// Size settings
float text_size;
float arrow_size;

// Cell shading settings
#define SHADE_HALFCELL_LINES 2 // How many lines to draw in the 2 halves of the cell
float shade_length;
float shade_spacing;

char text_buffer[4];

/*______________________________________________________________
@brief Called by Gamelevel during its initialization, this function will load
	   the values needed for rendering and such.
*/
void EnemyDisplayInit(void) {
	// Size of text is 20% of cell size
	text_size = GetCellSize() * 0.2f;
	// Size of movement arrow is 15% of cell size
	arrow_size = GetCellSize() * 0.15f;

	float offset = GetCellSize() * 0.5f - text_size;
	// Initialize the offset positions for the corners
	display[TOP_RIGHT] = CP_Vector_Set(offset, -offset);
	display[BOTTOM_RIGHT] = CP_Vector_Set(offset, offset);
	display[BOTTOM_LEFT] = CP_Vector_Set(offset, offset);
	display[TOP_LEFT] = CP_Vector_Set(offset, -offset);

	// Update shade rendering values
	shade_length = GetCellSize() * 0.15f; // Shade bar is 15% of cell each
	shade_spacing = (GetCellSize() - shade_length * SHADE_HALFCELL_LINES) / (SHADE_HALFCELL_LINES - 0.5f); // Spacing is remaining
}

/*______________________________________________________________
@brief Displays the enemy stats in the four corners of the cell
*/
void RenderEnemyDisplay(float pos_x, float pos_y, CP_Color color, int health, int max_health, int wall_damage) {
	CP_Settings_Fill(color);
	CP_Settings_TextSize(text_size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	// Render health
	if (health > 0) {
		CP_Settings_StrokeWeight(0.0f);
		CP_Settings_Fill(HEALTH_GREEN);
		CP_Graphics_DrawRect(pos_x - GetCellSize() * 0.4f, pos_y - GetCellSize() * 0.4f, GetCellSize() * 0.8f, GetCellSize() * 0.15f);
	}

	// Render wall damage if any
	if (wall_damage > 0) {
		CP_Settings_Fill(HEALTH_RED);
		sprintf_s(text_buffer, _countof(text_buffer), "%d", wall_damage);
		CP_Font_DrawText(text_buffer, pos_x + display[DAMAGE].x, pos_y + display[DAMAGE].y);
	}
}

/*______________________________________________________________
@brief 
*/
void RenderEnemyMovement(float pos_x, float pos_y, CP_Color color, int movement) {
	if (!movement) return;

	int actual_movement = 1, grid_x = PosXToGridX(pos_x), grid_y = PosYToGridY(pos_y);
	for (; actual_movement < movement; ++actual_movement) {

		// Redo logic, should hard stop on wall even if enemy in front
		EnemyInfo* type;
		if (type = GetAliveEnemyFromGrid(grid_x - actual_movement, grid_y)) {
			actual_movement += type->MovementSpeed;
			break;
		}
	}

	if (!actual_movement) return;

	color.a = 180;
	CP_Settings_Fill(color);
	CP_Settings_StrokeWeight(0.0f);
	//CP_Settings_TextSize(text_size);
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	CP_Vector anchor;

	int tile = 0;
	for (; tile < actual_movement; ++tile) {
		// CP_Font_DrawText(TEXT_MOVEMENT, pos_x - GetCellSize() * (tile + 0.5f), pos_y)
		anchor.x = pos_x - GetCellSize() * (tile + 0.5f);
		CP_Graphics_DrawTriangle(anchor.x - arrow_size, pos_y, anchor.x + arrow_size, pos_y + arrow_size, anchor.x + arrow_size, pos_y - arrow_size);
	}

	// If enemy is in cell, don't draw the shade
	EnemyInfo *type = GetAliveEnemyFromGrid(grid_x - actual_movement, grid_y);
	if (type != NULL) {
		// TODO REFACTOR WHEN ENEMY TYPES ADDED
		if (*(type->CharSprite + 1) == '\0') return;
	}

	color.a = 120;
	CP_Settings_Fill(color);
	// Shade cell,top half
	anchor.x = pos_x - GetCellSize() * (tile + 0.5f);
	anchor.y = pos_y - GetCellSize() * 0.5f;
	for (int index = 0; index < SHADE_HALFCELL_LINES; ++index) {
		CP_Graphics_DrawQuad(
			anchor.x + (shade_length + shade_spacing) * index, anchor.y,
			anchor.x + shade_length + (shade_length + shade_spacing) * index, anchor.y,
			anchor.x, anchor.y + shade_length + (shade_length + shade_spacing) * index,
			anchor.x, anchor.y + (shade_length + shade_spacing) * index
			);
	}

	// Shade cell, bottom half
	anchor.x = pos_x - GetCellSize() * (tile - 0.5f);
	anchor.y = pos_y + GetCellSize() * 0.5f;
	for (int index = 0; index < SHADE_HALFCELL_LINES; ++index) {
		CP_Graphics_DrawQuad(
			anchor.x - (shade_length + shade_spacing) * index, anchor.y,
			anchor.x - shade_length - (shade_length + shade_spacing) * index, anchor.y,
			anchor.x, anchor.y - shade_length - (shade_length + shade_spacing) * index,
			anchor.x, anchor.y - (shade_length + shade_spacing) * index
		);
	}
}
