/*!
@file	  Tutorial.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     14/11/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include "ColorTable.h" // For text colors
#include "WaveSystem.h" // For the tutorial zombie
#include "EnemyStats.h" // For zombie type
#include "Grid.h" // For grid conversion

#include "Tutorial.h"
#include "GameLoop.h"

static float tut_time_elapsed;
float box_stroke;

/*______________________________________________________________
@brief Timing settings, change the values here to change the animation.
*/
float time_skip_tut_start = 0.0f;
float time_p_intro_start = 1.75f;
float time_z_intro_start = 4.0f;

// Tutorial Texts
TutText text_skip_tut;
TutText text_p_intro;
TutText text_z_intro;

// Tutorial Arrows
TutArrow arrow_p_intro;
TutArrow arrow_z_intro;

#pragma region
void TutorialUpdate(void);
void TutorialExit(void);
TutText TutTextCreate(char* message, CP_Vector pos, CP_Color color, float size, CP_Vector box_bounds, CP_Vector box_size);
TutArrow TutArrowCreate(CP_Vector pos_start, CP_Vector pos_end, CP_Color color, float thickness);
void DrawArrow(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness);
void DrawTextFull(char* text, float pos_x, float pos_y, CP_Color color, CP_TEXT_ALIGN_HORIZONTAL h_align, CP_TEXT_ALIGN_VERTICAL v_align, float size);
void DrawTextTopLeft(char* text, float pos_x, float pos_y, CP_Color color, float size);
void DrawTextCentre(char* text, float pos_x, float pos_y, CP_Color color, float size);
void DrawTextBox(float pos_x, float pos_y, float size_x, float size_y);
#pragma endregion Forward Declarations

void TutorialInit(void) {
	SubscribeEvent(PLAYER_UPDATE, TutorialUpdate, 2);
	SubscribeEvent(PLAYER_END, TutorialExit, DEFAULT_PRIORITY);

	// Initialize the texts
	CP_Vector box_bounds, box_size;
	CP_Vector spawn_pos, end_pos;
	float width = CP_System_GetWindowWidth(), height = CP_System_GetWindowHeight();
	float text_size = height / 40.0f;
	float arrow_size = height / 80.0f;
	box_stroke = arrow_size / 3.0f;

	// Above center of screen
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 4.0f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 35.0f;
	box_size.y = text_size * 2.8f;
	text_skip_tut = TutTextCreate("<Play any piece to skip tutorial>", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);

	// Slightly below wizard pos (taken from Wizard.h)
	spawn_pos.x = width / 9.0f;
	spawn_pos.y = height / 1.6f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 13.0f;
	box_size.y = text_size * 2.8f;
	text_p_intro = TutTextCreate("This is you.", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);
	spawn_pos.x = width / 9.0f;
	spawn_pos.y = height / 1.75f;
	end_pos.x = width / 14.0f;
	end_pos.y = height / 2.05f;
	arrow_p_intro = TutArrowCreate(spawn_pos, end_pos, TUTORIAL_COLOR, arrow_size);

	// Spawn the tutorial zombie below the center!
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.75f;
	SpawnEnemyInCell(PosXToGridX(spawn_pos.x), PosYToGridY(spawn_pos.y), GetEnemyPrefab(1));

	// Slightly below the tutorial zombie pos
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.6f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 40.0f;
	box_size.y = text_size * 2.8f;
	text_z_intro = TutTextCreate("Zombies are coming to eat your brains!", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);
	spawn_pos.x = width / 9.0f;
	spawn_pos.y = height / 1.75f;
	end_pos.x = width / 14.0f;
	end_pos.y = height / 2.05f;
	arrow_z_intro = TutArrowCreate(spawn_pos, end_pos, TUTORIAL_COLOR, arrow_size);

	tut_time_elapsed = 0.0f;
}

/*______________________________________________________________
@brief The sequence of events to show for the tutorial
*/
void TutorialUpdate(void) {
	if (time_skip_tut_start < tut_time_elapsed) {
		DrawTextBox(text_skip_tut.box_bounds.x, text_skip_tut.box_bounds.y, text_skip_tut.box_size.x, text_skip_tut.box_size.y);
		DrawTextCentre(text_skip_tut.text, text_skip_tut.pos.x, text_skip_tut.pos.y, text_skip_tut.color, text_skip_tut.size);
	}

	if (time_p_intro_start < tut_time_elapsed) {
		DrawTextBox(text_p_intro.box_bounds.x, text_p_intro.box_bounds.y, text_p_intro.box_size.x, text_p_intro.box_size.y);
		DrawTextCentre(text_p_intro.text, text_p_intro.pos.x, text_p_intro.pos.y, text_p_intro.color, text_p_intro.size);
		DrawArrow(arrow_p_intro.pos_start.x, arrow_p_intro.pos_start.y, arrow_p_intro.pos_end.x, arrow_p_intro.pos_end.y, arrow_p_intro.color, arrow_p_intro.thickness);
	}

	if (time_z_intro_start < tut_time_elapsed) {
		DrawTextBox(text_z_intro.box_bounds.x, text_z_intro.box_bounds.y, text_z_intro.box_size.x, text_z_intro.box_size.y);
		DrawTextCentre(text_z_intro.text, text_z_intro.pos.x, text_z_intro.pos.y, text_z_intro.color, text_z_intro.size);
	}

	tut_time_elapsed += CP_System_GetDt();
}

void TutorialExit(void) {
	// Hide the tutorial upon completion of the player's first turn
	UnsubscribeEvent(PLAYER_UPDATE, TutorialUpdate);
	UnsubscribeEvent(PLAYER_END, TutorialExit);
}

//______________________________________________________________
// Initialization functions
TutText TutTextCreate(char* message, CP_Vector pos, CP_Color color, float size, CP_Vector box_bounds, CP_Vector box_size) {
	TutText new_text = { .text = message, .pos = pos, .color = color, .size = size, .box_bounds = box_bounds, .box_size = box_size };
	return new_text;
}

TutArrow TutArrowCreate(CP_Vector pos_start, CP_Vector pos_end, CP_Color color, float thickness) {
	TutArrow new_arrow = { .pos_start = pos_start, .pos_end = pos_end, .color = color, .thickness = thickness };
	return new_arrow;
}

//______________________________________________________________
// Rendering functions

void DrawArrow(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness) {
	CP_Vector line = CP_Vector_Set(point_1_x - point_2_x, point_1_y - point_2_y);
	line = CP_Vector_Normalize(line);
	line = CP_Vector_Scale(line, 24.0f); // Second parameter is size of triangle at the end of the arrow

	// Draw pointed part
	CP_Vector normal = CP_Vector_Set(-line.y, line.x);

	CP_Settings_StrokeWeight(.0f);
	CP_Settings_Fill(color);
	// The 1.5f bias makes the arrow longer than wider
	CP_Graphics_DrawTriangle(point_2_x, point_2_y, point_2_x + line.x * 1.5f + normal.x, point_2_y + line.y * 1.5f + normal.y, point_2_x + line.x * 1.5f - normal.x, point_2_y + line.y * 1.5f - normal.y);

	// Draw line part
	CP_Settings_Stroke(color);
	CP_Settings_StrokeWeight(thickness);
	CP_Graphics_DrawLine(point_1_x, point_1_y, point_2_x + line.x, point_2_y + line.y);
}

void DrawTextFull(char* text, float pos_x, float pos_y, CP_Color color, CP_TEXT_ALIGN_HORIZONTAL h_align, CP_TEXT_ALIGN_VERTICAL v_align, float size) {
	CP_Settings_TextAlignment(h_align, v_align);
	CP_Settings_TextSize(size);
	CP_Settings_Fill(color);
	CP_Font_DrawText(text, pos_x, pos_y);
}

void DrawTextTopLeft(char* text, float pos_x, float pos_y, CP_Color color, float size) {
	DrawTextFull(text, pos_x, pos_y, color, CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP, size);
}

void DrawTextCentre(char* text, float pos_x, float pos_y, CP_Color color, float size) {
	DrawTextFull(text, pos_x, pos_y, color, CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE, size);
}

void DrawTextBox(float pos_x, float pos_y, float size_x, float size_y) {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(BLACK);
	CP_Settings_Stroke(TUTORIAL_COLOR);
	CP_Settings_StrokeWeight(box_stroke);
	CP_Graphics_DrawRect(pos_x, pos_y, size_x, size_y);
}
