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

#include "GameLoop.h" // For player update
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

// Movement settings
float move_elapsed_time;
float move_blink_speed = 0.5f;
_Bool move_draw;

CP_Image attack_icon;

char text_buffer[4];

// Zombie types description array
E_DisplayInfo zombie_info[ZOMBIE_TYPE_LENGTH];

float hover_info_display_time = 1.0f;
float hover_elapsed_time;
EnemyInfo *hovered_zombie;
int last_hover_x, last_hover_y;
E_DisplayText info_text;

#pragma region
void EnemyDisplayTimeIncrement(void);
void ResetDisplayEnemyInfoTime(void);
#pragma endregion Forward Declarations

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

	// Load attack icon
	attack_icon = CP_Image_Load("Assets/AttackIcon.png");

	move_elapsed_time = 0.0f;
	move_draw = FALSE;

	// Subscribe the enemy movement flicker time incrementation to player update
	SubscribeEvent(PLAYER_UPDATE, EnemyDisplayTimeIncrement, 0);

	//______________________________________________________________
	// Initialize zombie info descriptions
	hover_elapsed_time = 0.0f;
	last_hover_x = last_hover_y = 0;
	hovered_zombie = NULL;
	SubscribeEvent(PLAYER_END, ResetDisplayEnemyInfoTime, 0);

	zombie_info[WALL].type = GetEnemyPrefab(0); // 0 is wall
	zombie_info[WALL].description = "Blocks zombie movement... until it gets destroyed.";
	zombie_info[WALL].char_count = 50;

	zombie_info[ZOMBIE].type = GetEnemyPrefab(1); // 1 is zombie
	zombie_info[ZOMBIE].description = "Average zombie, strength in numbers.";
	zombie_info[ZOMBIE].char_count = 36;

	zombie_info[LEAPER].type = GetEnemyPrefab(2); // 2 is leaper
	zombie_info[LEAPER].description = "Leaper zombie, but has weak attack against walls.";
	zombie_info[LEAPER].char_count = 49;

	zombie_info[TANK].type = GetEnemyPrefab(3); // 3 is tank
	zombie_info[TANK].description = "Tank zombie. Can take a lot of hits before dying.";
	zombie_info[TANK].char_count = 49;

	zombie_info[BREAKER].type = GetEnemyPrefab(4); // 4 is breaker
	zombie_info[BREAKER].description = "Breacher zombie, Destroys walls like paper mache.";
	zombie_info[BREAKER].char_count = 49;

	zombie_info[GRAVE].type = GetEnemyPrefab(5); // 5 is grave
	zombie_info[GRAVE].description = "Grave. A wildcard. Can spawn any zombie next turn.";
	zombie_info[GRAVE].char_count = 50;

	// Display zombie info text box
	info_text.offset = CP_Vector_Set(cell_size / 2.0f, cell_size / 1.5f);
	info_text.size = CP_System_GetWindowHeight() / 60.0f;
	info_text.stroke = CP_System_GetWindowHeight() / 240.0f;
}

void EnemyDisplayTimeIncrement(void) {
	// Update zombie move indicator flicker
	move_elapsed_time += CP_System_GetDt();

	if (move_elapsed_time > move_blink_speed) {
		move_draw = TRUE;

		if (move_elapsed_time > move_blink_speed * 2) {
			move_draw = FALSE;
			move_elapsed_time = 0.0f;
		}
	}

	// Update zombie information hover
	if (!IsIndexInPlayingArea(0, PosYToGridY(CP_Input_GetMouseY()))) {
		hovered_zombie = NULL;
		return;
	}

	if (last_hover_x == PosXToGridX(CP_Input_GetMouseX()) && last_hover_y == PosYToGridY(CP_Input_GetMouseY())) {
		hover_elapsed_time += CP_System_GetDt();

		if (hover_elapsed_time > hover_info_display_time) {
			if (!hovered_zombie) {
				if (hovered_zombie = GetAliveEnemyFromGrid(last_hover_x, last_hover_y)) {
					info_text.zombie_index = hovered_zombie->type;
				}
			}
		}
	}
	else {
		ResetDisplayEnemyInfoTime();
		last_hover_x = PosXToGridX(CP_Input_GetMouseX());
		last_hover_y = PosYToGridY(CP_Input_GetMouseY());
		hovered_zombie = NULL;
	}
}

/*______________________________________________________________
@brief Displays the enemy stats in the four corners of the cell
*/
void RenderEnemyDisplay(float pos_x, float pos_y, CP_Color color, int health, int max_health, int wall_damage) {
	CP_Settings_RectMode(CP_POSITION_CORNER);

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
		for (int index = 0; index < wall_damage; ++index) {
			CP_Image_Draw(attack_icon, pos_x + display[DAMAGE].x - (health_spacing + text_size) * index, pos_y + display[DAMAGE].y, text_size, text_size, 255);
		}

		//CP_Settings_TextSize(text_size);
		//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

		//CP_Settings_Fill(HEALTH_RED);
		//sprintf_s(text_buffer, _countof(text_buffer), "%d", wall_damage);
		//CP_Font_DrawText(text_buffer, pos_x + display[DAMAGE].x, pos_y + display[DAMAGE].y);
	}
}

/*______________________________________________________________
@brief 
*/
void RenderEnemyMovement(float pos_x, float pos_y, CP_Color color, int movement) {
	if (!movement || !move_draw) return;

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

void FreeEnemyDisplayIcon(void) {
	CP_Image_Free(&attack_icon);
}

void DisplayEnemyInfo(void) {
	if (!hovered_zombie) return;

	// Text box
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(BLACK);
	CP_Settings_Stroke(TUTORIAL_COLOR);
	CP_Settings_StrokeWeight(info_text.stroke);
	CP_Graphics_DrawRect(CP_Input_GetMouseX() + info_text.offset.x, CP_Input_GetMouseY() + info_text.offset.y, info_text.size * (zombie_info[info_text.zombie_index].char_count + 2), info_text.size * 4);

	// Text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(info_text.size);
	CP_Settings_Fill(TUTORIAL_COLOR);

	// Enemy stats
	char text_buffer[28];
	sprintf_s(text_buffer, _countof(text_buffer), "HP:%d/%d WALL_ATK:%d SPD:%d", hovered_zombie->Health, hovered_zombie->MaxHealth, hovered_zombie->damage, hovered_zombie->MovementSpeed);
	CP_Font_DrawText(text_buffer, CP_Input_GetMouseX() + info_text.offset.x, CP_Input_GetMouseY() + info_text.offset.y - info_text.size / 1.2f);

	// Enemy description
	CP_Font_DrawText(zombie_info[info_text.zombie_index].description, CP_Input_GetMouseX() + info_text.offset.x, CP_Input_GetMouseY() + info_text.offset.y + info_text.size / 1.2f);
}

void ResetDisplayEnemyInfoTime(void) {
	hover_elapsed_time = 0.0f;
}
