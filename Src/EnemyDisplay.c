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

// Numbers to display in the four corners of the cell
CP_Vector display[4];

// Size settings
float cell_size;
float text_size;
float arrow_size;

// Cell shading settings
#define SHADE_HALFCELL_LINES 2 // How many lines to draw in the 2 halves of the cell
float shade_length;
float shade_spacing;

// Health bar settings
CP_Vector health_offset;
float health_height;
float health_width;
float health_spacing; // The gap between each health bar

char text_buffer[4];

/*______________________________________________________________
@brief Called by Gamelevel during its initialization, this function will load
	   the values needed for rendering and such.
*/
void EnemyDisplayInit(void) {
	cell_size = GetCellSize();

	// Size of text is 20% of cell size
	text_size = cell_size * 0.2f;
	// Size of movement arrow is 15% of cell size
	arrow_size = cell_size * 0.15f;

	float offset = cell_size * 0.5f - text_size;
	// Initialize the offset positions for the corners
	display[TOP_RIGHT] = CP_Vector_Set(offset, -offset);
	display[BOTTOM_RIGHT] = CP_Vector_Set(offset, offset);
	display[BOTTOM_LEFT] = CP_Vector_Set(offset, offset);
	display[TOP_LEFT] = CP_Vector_Set(offset, -offset);

	// Update shade rendering values
	shade_length = cell_size * 0.15f; // Shade bar is 15% of cell each
	shade_spacing = (cell_size - shade_length * SHADE_HALFCELL_LINES) / (SHADE_HALFCELL_LINES - 0.5f); // Spacing is remaining

	// Update health rendering values
	health_height = cell_size * 0.1f; // Health bar is 10% of cell height
	health_width = cell_size * 0.8f; // Health bar is 80% of cell width
	health_spacing = cell_size * 0.05f; // The gap between each health bar cell is 5% of cell width
	health_offset.x = cell_size * 0.4f; // Offset places bar at top of cell
	health_offset.y = cell_size * 0.4f;
}

/*______________________________________________________________
@brief Displays the enemy stats in the four corners of the cell
*/
void RenderEnemyDisplay(float pos_x, float pos_y, CP_Color color, int health, int max_health, int wall_damage) {
	// Render health
	if (health > 0) {
		CP_Settings_StrokeWeight(0.0f);

		// Color of health depends on percentage
		float health_percent = (float)health / (float)max_health;
		if (health_percent > 0.99f) {
			CP_Settings_Fill(HEALTH_GREEN);
		}
		else if (health_percent > 0.5f) {
			CP_Settings_Fill(HEALTH_YELLOW);
		}
		else {
			CP_Settings_Fill(HEALTH_RED);
		}

		// Skip calculations if health is just 1
		if (max_health == 1) {
			CP_Graphics_DrawRect(pos_x - health_offset.x, pos_y - health_offset.y, health_width, health_height);
		}
		else {
			// Size of each bar in health depends on number of bars
			float bar_cell_length = (health_width - health_spacing * (max_health - 1)) / max_health;
			for (int count = 0; count < health; ++count) {
				CP_Graphics_DrawRect(pos_x - health_offset.x + (bar_cell_length + health_spacing) * count, pos_y - health_offset.y, bar_cell_length, health_height);
			}
		}
	}

	// Render wall damage if any
	if (wall_damage > 0) {
		CP_Settings_TextSize(text_size);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

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

	CP_Settings_Fill(ENEMY_MOVEMENT);

	int actual_movement = 1, grid_x = PosXToGridX(pos_x), grid_y = PosYToGridY(pos_y);
	for (; actual_movement < movement; ++actual_movement) {
		EnemyInfo* enemy;
		if (enemy = GetAliveEnemyFromGrid(grid_x - actual_movement, grid_y)) {
			if (enemy->type == WALL) {
				break;
			}
			else {
				if (enemy->MovementSpeed + actual_movement > movement) {
					actual_movement = movement;
					break;
				}
				else {
					actual_movement += enemy->MovementSpeed - 1;
					break;
				}
			}
		}
	}

	if (!actual_movement) return;

	CP_Settings_StrokeWeight(0.0f);

	CP_Vector anchor;

	int tile = 0;
	for (; tile < actual_movement; ++tile) {
		anchor.x = pos_x - cell_size * (tile + 0.5f);
		CP_Graphics_DrawTriangle(anchor.x - arrow_size, pos_y, anchor.x + arrow_size, pos_y + arrow_size, anchor.x + arrow_size, pos_y - arrow_size);
	}

	// If enemy is in cell, don't draw the shade
	EnemyInfo *enemy = GetAliveEnemyFromGrid(grid_x - actual_movement, grid_y);
	if (enemy != NULL) {
		if (enemy->type != WALL) return;
	}

	// Shade cell,top half
	anchor.x = pos_x - cell_size * (tile + 0.5f);
	anchor.y = pos_y - cell_size * 0.5f;
	for (int index = 0; index < SHADE_HALFCELL_LINES; ++index) {
		CP_Graphics_DrawQuad(
			anchor.x + (shade_length + shade_spacing) * index, anchor.y,
			anchor.x + shade_length + (shade_length + shade_spacing) * index, anchor.y,
			anchor.x, anchor.y + shade_length + (shade_length + shade_spacing) * index,
			anchor.x, anchor.y + (shade_length + shade_spacing) * index
			);
	}

	// Shade cell, bottom half
	anchor.x = pos_x - cell_size * (tile - 0.5f);
	anchor.y = pos_y + cell_size * 0.5f;
	for (int index = 0; index < SHADE_HALFCELL_LINES; ++index) {
		CP_Graphics_DrawQuad(
			anchor.x - (shade_length + shade_spacing) * index, anchor.y,
			anchor.x - shade_length - (shade_length + shade_spacing) * index, anchor.y,
			anchor.x, anchor.y - shade_length - (shade_length + shade_spacing) * index,
			anchor.x, anchor.y - (shade_length + shade_spacing) * index
		);
	}
}
