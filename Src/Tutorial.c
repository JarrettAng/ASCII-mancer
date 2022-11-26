/*!
@file	  Tutorial.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@date     22/11/2022
@brief    This source file contains the timings and positions for all the tutorial objects, as well as 10 functions,

		  TutorialInit - Sets up the text and other UI elements, and event subscription to player update and end.
		  TutorialUpdate - Renders all the tutorial UI elements like the arrows & text.
		  TutorialExit - When a piece is played, end the tutorial by unsubscribing the render function from update.

		  TutTextCreate - Loads the information for rendering text, like position, size, color into the TutText struct.
		  TutArrowCreate - Loads the information for rendering arrows, like position, size, color into the TutArrow struct.

		  DrawArrow - Renders the arrow based on the information in TutArrow.
		  DrawTextFull - Renders the text based on the information in TutText.
		  DrawTextTopLeft - Renders the text with its anchor point on the top left.
		  DrawTextCentre - Renders the text with its anchor point in the center.
		  DrawTextBox - Renders the background box for the text.
________________________________________________________________________________________________________*/

#include "ColorTable.h" // For text colors
#include "WaveSystem.h" // For the tutorial zombie
#include "EnemyStats.h" // For zombie type
#include "Grid.h"		// For grid conversion

#include "Tutorial.h"
#include "GameLoop.h"	// For subscribing to player's turn end

static float tut_time_elapsed;		// Time passed since start of tutorial
_Bool timer_paused;					// Is the tutorial paused right now?
static float pause_time_elapsed;	// Time passed since tutorial was paused
float pause_blink_speed = 0.5f;		// How often the "Click to continue" text should blink
float box_stroke, arrow_head;		// Sizes for the rendering of tutorial objects

/*______________________________________________________________
@brief Timing settings, change the values here to change the animation.
*/
float time_skip_tut_start = 0.0f;		// Skip tutorial text
float time_p_intro_start = 1.75f;		// This is the player text
float time_z_intro_start = 4.0f;		// These are zombies text
float time_hover_intro_start = 7.25f;	// Hover over zombies for more information text
float time_section_1_wait = 12.0f;		// Click to continue text
float time_section_1_end = 12.5f;

float time_piece_start = 13.0f;			// Tetris pieces introduction text
float time_attack_start = 15.5f;		// Attack pieces introduction text
float time_defend_start = 18.0f;		// Defense pieces introduction text
float time_section_2_wait = 20.0f;		// Click to continue text
float time_section_2_end = 20.5f;

float time_queue_start = 21.0f;			// Player's hand queue introduction text
float time_drag_start = 24.5f;			// Click and drag controls text
float time_rotate_start = 27.0f;		// Right click to rotate text
float time_tut_end_start = 30.0f;		// Play a piece to start game text

// Tutorial Texts, contains all the information for rendering
TutText text_skip_tut;
TutText text_p_intro;
TutText text_z_intro;
TutText text_hover_intro;
TutText text_piece_intro;
TutText text_defend_intro;
TutText text_attack_intro;
TutText text_queue_intro;
TutText text_drag_intro;
TutText text_rotate_intro;
TutText text_end_intro;
TutText text_continue;

// Tutorial Arrows, contains all the information for rendering
TutArrow arrow_p_intro;
TutArrow arrow_z_intro;
TutArrow arrow_attack_intro;
TutArrow arrow_defend_intro;
TutArrow arrow_drag_intro;

#pragma region
void TutorialUpdate(void);
void TutorialExit(void);
TutText TutTextCreate(char* message, CP_Vector pos, CP_Color color, float size, CP_Vector box_bounds, CP_Vector box_size);
TutArrow TutArrowCreate(CP_Vector pos_start, CP_Vector pos_end, CP_Color color, float thickness);
void DrawArrow(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness);
void DrawTextFull(char* text, float pos_x, float pos_y, CP_Color color, CP_TEXT_ALIGN_HORIZONTAL h_align, CP_TEXT_ALIGN_VERTICAL v_align, float size);
void DrawTextTopLeft(char* text, float pos_x, float pos_y, CP_Color color, float size);
void DrawTextCentre(char* text, float pos_x, float pos_y, CP_Color color, float size);
void DrawTextBox(float pos_x, float pos_y, float size_x, float size_y, CP_Color color);
#pragma endregion Forward Declarations

/*______________________________________________________________
@brief Should be called on game level load, sets up the text and other UI elements,
	   and event subscription to player update and end.
*/
void TutorialInit(void) {
	SubscribeEvent(PLAYER_UPDATE, TutorialUpdate, 2);
	SubscribeEvent(PLAYER_END, TutorialExit, DEFAULT_PRIORITY);

	// Initialize the texts
	CP_Vector box_bounds = CP_Vector_Zero(), box_size = CP_Vector_Zero();
	CP_Vector spawn_pos = CP_Vector_Zero(), end_pos = CP_Vector_Zero();
	float width = (float)CP_System_GetWindowWidth(), height = (float)CP_System_GetWindowHeight();
	float text_size = height / 40.0f;
	float arrow_size = height / 80.0f;
	box_stroke = arrow_size / 3.0f;
	arrow_head = arrow_size * 1.5f;

	//______________________________________________________________
	// Click to continue text
	// Above center of screen
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 3.0f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 35.0f;
	box_size.y = text_size * 2.8f;
	text_continue = TutTextCreate("Click anywhere to continue...", spawn_pos, MENU_RED, text_size, box_bounds, box_size);

	//______________________________________________________________
	// Skip tutorial text
	// Above center of screen
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 4.0f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 35.0f;
	box_size.y = text_size * 2.8f;
	text_skip_tut = TutTextCreate("<Play any piece to skip tutorial>", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);

	//______________________________________________________________
	// Player introduction text
	// Slightly below wizard pos (taken from Wizard.h)
	spawn_pos.x = width / 9.0f;
	spawn_pos.y = height / 1.6f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 13.0f;
	box_size.y = text_size * 2.8f;
	text_p_intro = TutTextCreate("This is you.", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);
	spawn_pos.x = width / 8.0f;
	spawn_pos.y = height / 1.75f;
	end_pos.x = width / 14.0f;
	end_pos.y = height / 2.05f;
	arrow_p_intro = TutArrowCreate(spawn_pos, end_pos, TUTORIAL_COLOR, arrow_size);

	//______________________________________________________________
	// Spawn the tutorial zombies below the center!
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.75f;
	SpawnEnemyInCell(PosXToGridX(spawn_pos.x * 0.9f), PosYToGridY(spawn_pos.y), GetEnemyPrefab(1));
	SpawnEnemyInCell(PosXToGridX(spawn_pos.x * 0.9f), PosYToGridY(spawn_pos.y * 0.8f), GetEnemyPrefab(1));

	SpawnEnemyInCell(PosXToGridX(spawn_pos.x * 1.4f), PosYToGridY(spawn_pos.y * 0.8f), GetEnemyPrefab(2));
	SpawnEnemyInCell(PosXToGridX(spawn_pos.x * 1.3f), PosYToGridY(spawn_pos.y * 0.8f), GetEnemyPrefab(2));

	//______________________________________________________________
	// Zombie introduction text
	// Slightly below the tutorial zombie pos
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.6f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 40.0f;
	box_size.y = text_size * 2.8f;
	text_z_intro = TutTextCreate("Zombies are coming to eat your brains!", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.75f;
	end_pos.x = width / 2.15f;
	end_pos.y = height / 1.85f;
	arrow_z_intro = TutArrowCreate(spawn_pos, end_pos, TUTORIAL_COLOR, arrow_size);

	//______________________________________________________________
	// Hover introduction text
	// Slightly below the zombie introduction text
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.4f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 52.0f;
	box_size.y = text_size * 2.8f;
	text_hover_intro = TutTextCreate("You can hover over zombies for more info anytime.", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);

	//______________________________________________________________
	// Tetris piece introduction text
	// Below center of the screen
	spawn_pos.x = width / 2.1f;
	spawn_pos.y = height / 1.6f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 40.0f;
	box_size.y = text_size * 2.8f;
	text_piece_intro = TutTextCreate("Use Tetris pieces to defend yourself!", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);

	//______________________________________________________________
	// Attack piece introduction text
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.4f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 23.0f;
	box_size.y = text_size * 2.8f;
	text_attack_intro = TutTextCreate("These damage zombies.", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 1.35f;
	end_pos.x = width / 2.1f;
	end_pos.y = height / 1.27f;
	arrow_attack_intro = TutArrowCreate(spawn_pos, end_pos, TUTORIAL_COLOR, arrow_size);

	//______________________________________________________________
	// Defense piece introduction text
	spawn_pos.x = width / 5.5f;
	spawn_pos.y = height / 1.4f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 18.0f;
	box_size.y = text_size * 2.8f;
	text_defend_intro = TutTextCreate("This build walls.", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);
	spawn_pos.x = width / 4.5f;
	spawn_pos.y = height / 1.35f;
	end_pos.x = width / 4.0f;
	end_pos.y = height / 1.27f;
	arrow_defend_intro = TutArrowCreate(spawn_pos, end_pos, TUTORIAL_COLOR, arrow_size);

	//______________________________________________________________
	// Queue introduction text
	// Right of queue (peek) area
	spawn_pos.x = width / 1.5f;
	spawn_pos.y = height / 1.4f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 34.0f;
	box_size.y = text_size * 2.8f;
	text_queue_intro = TutTextCreate("<< New pieces come from the right", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);

	//______________________________________________________________
	// Drag introduction text
	spawn_pos.x = width / 1.47f;
	spawn_pos.y = height / 1.6f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 32.0f;
	box_size.y = text_size * 2.8f;
	text_drag_intro = TutTextCreate("Click & drag to use the pieces!", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);
	spawn_pos.x = width / 2.75f;
	spawn_pos.y = height / 1.27f;
	end_pos.x = width / 2.25f;
	end_pos.y = height / 1.75f;
	arrow_drag_intro = TutArrowCreate(spawn_pos, end_pos, TUTORIAL_COLOR, arrow_size);

	//______________________________________________________________
	// Rotate introduction text
	spawn_pos.x = width / 1.345f;
	spawn_pos.y = height / 1.87f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 23.0f;
	box_size.y = text_size * 2.8f;
	text_rotate_intro = TutTextCreate("Right click to rotate.", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);

	//______________________________________________________________
	// End tutorial text
	// Above center of screen
	spawn_pos.x = width / 2.0f;
	spawn_pos.y = height / 4.0f;
	box_bounds.x = spawn_pos.x;
	box_bounds.y = spawn_pos.y;
	box_size.x = text_size * 34.0f;
	box_size.y = text_size * 2.8f;
	text_end_intro = TutTextCreate("Tutorial ends here, drag a piece!", spawn_pos, TUTORIAL_COLOR, text_size, box_bounds, box_size);

	tut_time_elapsed = 0.0f;
	timer_paused = FALSE;
	pause_time_elapsed = 0.0f;
}

/*______________________________________________________________
@brief Renders all the tutorial UI elements like the arrows & text, should be called in an update loop.
	   The sequence of events to show for the tutorial
*/
void TutorialUpdate(void) {
	if (time_section_1_end > tut_time_elapsed) {
		if (time_skip_tut_start < tut_time_elapsed && tut_time_elapsed < time_section_1_wait) {
			DrawTextBox(text_skip_tut.box_bounds.x, text_skip_tut.box_bounds.y, text_skip_tut.box_size.x, text_skip_tut.box_size.y, text_skip_tut.color);
			DrawTextCentre(text_skip_tut.text, text_skip_tut.pos.x, text_skip_tut.pos.y, text_skip_tut.color, text_skip_tut.size);
		}

		if (time_p_intro_start < tut_time_elapsed) {
			DrawTextBox(text_p_intro.box_bounds.x, text_p_intro.box_bounds.y, text_p_intro.box_size.x, text_p_intro.box_size.y, text_p_intro.color);
			DrawTextCentre(text_p_intro.text, text_p_intro.pos.x, text_p_intro.pos.y, text_p_intro.color, text_p_intro.size);
			DrawArrow(arrow_p_intro.pos_start.x, arrow_p_intro.pos_start.y, arrow_p_intro.pos_end.x, arrow_p_intro.pos_end.y, arrow_p_intro.color, arrow_p_intro.thickness);
		}

		if (time_z_intro_start < tut_time_elapsed) {
			DrawTextBox(text_z_intro.box_bounds.x, text_z_intro.box_bounds.y, text_z_intro.box_size.x, text_z_intro.box_size.y, text_z_intro.color);
			DrawTextCentre(text_z_intro.text, text_z_intro.pos.x, text_z_intro.pos.y, text_z_intro.color, text_z_intro.size);
			DrawArrow(arrow_z_intro.pos_start.x, arrow_z_intro.pos_start.y, arrow_z_intro.pos_end.x, arrow_z_intro.pos_end.y, arrow_z_intro.color, arrow_z_intro.thickness);
		}

		if (time_hover_intro_start < tut_time_elapsed) {
			DrawTextBox(text_hover_intro.box_bounds.x, text_hover_intro.box_bounds.y, text_hover_intro.box_size.x, text_hover_intro.box_size.y, text_hover_intro.color);
			DrawTextCentre(text_hover_intro.text, text_hover_intro.pos.x, text_hover_intro.pos.y, text_hover_intro.color, text_hover_intro.size);
		}

		if (time_section_1_wait < tut_time_elapsed) {
			timer_paused = TRUE;
		}
	}
	else if (time_section_2_end > tut_time_elapsed) {
		if (time_piece_start < tut_time_elapsed) {
			DrawTextBox(text_piece_intro.box_bounds.x, text_piece_intro.box_bounds.y, text_piece_intro.box_size.x, text_piece_intro.box_size.y, text_piece_intro.color);
			DrawTextCentre(text_piece_intro.text, text_piece_intro.pos.x, text_piece_intro.pos.y, text_piece_intro.color, text_piece_intro.size);
		}

		if (time_attack_start < tut_time_elapsed) {
			DrawTextBox(text_attack_intro.box_bounds.x, text_attack_intro.box_bounds.y, text_attack_intro.box_size.x, text_attack_intro.box_size.y, text_attack_intro.color);
			DrawTextCentre(text_attack_intro.text, text_attack_intro.pos.x, text_attack_intro.pos.y, text_attack_intro.color, text_attack_intro.size);
			DrawArrow(arrow_attack_intro.pos_start.x, arrow_attack_intro.pos_start.y, arrow_attack_intro.pos_end.x, arrow_attack_intro.pos_end.y, arrow_attack_intro.color, arrow_attack_intro.thickness);
		}

		if (time_defend_start < tut_time_elapsed) {
			DrawTextBox(text_defend_intro.box_bounds.x, text_defend_intro.box_bounds.y, text_defend_intro.box_size.x, text_defend_intro.box_size.y, text_defend_intro.color);
			DrawTextCentre(text_defend_intro.text, text_defend_intro.pos.x, text_defend_intro.pos.y, text_defend_intro.color, text_defend_intro.size);
			DrawArrow(arrow_defend_intro.pos_start.x, arrow_defend_intro.pos_start.y, arrow_defend_intro.pos_end.x, arrow_defend_intro.pos_end.y, arrow_defend_intro.color, arrow_defend_intro.thickness);
		}

		if (time_section_2_wait < tut_time_elapsed) {
			timer_paused = TRUE;
		}
	}
	else {
		if (time_queue_start < tut_time_elapsed) {
			DrawTextBox(text_queue_intro.box_bounds.x, text_queue_intro.box_bounds.y, text_queue_intro.box_size.x, text_queue_intro.box_size.y, text_queue_intro.color);
			DrawTextCentre(text_queue_intro.text, text_queue_intro.pos.x, text_queue_intro.pos.y, text_queue_intro.color, text_queue_intro.size);
		}
		if (time_drag_start < tut_time_elapsed) {
			DrawTextBox(text_drag_intro.box_bounds.x, text_drag_intro.box_bounds.y, text_drag_intro.box_size.x, text_drag_intro.box_size.y, text_drag_intro.color);
			DrawTextCentre(text_drag_intro.text, text_drag_intro.pos.x, text_drag_intro.pos.y, text_drag_intro.color, text_drag_intro.size);
			DrawArrow(arrow_drag_intro.pos_start.x, arrow_drag_intro.pos_start.y, arrow_drag_intro.pos_end.x, arrow_drag_intro.pos_end.y, arrow_drag_intro.color, arrow_drag_intro.thickness);
		}
		if (time_rotate_start < tut_time_elapsed) {
			DrawTextBox(text_rotate_intro.box_bounds.x, text_rotate_intro.box_bounds.y, text_rotate_intro.box_size.x, text_rotate_intro.box_size.y, text_rotate_intro.color);
			DrawTextCentre(text_rotate_intro.text, text_rotate_intro.pos.x, text_rotate_intro.pos.y, text_rotate_intro.color, text_rotate_intro.size);
		}
		if (time_tut_end_start < tut_time_elapsed) {
			DrawTextBox(text_end_intro.box_bounds.x, text_end_intro.box_bounds.y, text_end_intro.box_size.x, text_end_intro.box_size.y, text_end_intro.color);
			DrawTextCentre(text_end_intro.text, text_end_intro.pos.x, text_end_intro.pos.y, text_end_intro.color, text_end_intro.size);
		}
	}

	if (timer_paused) {
		DrawTextBox(text_continue.box_bounds.x, text_continue.box_bounds.y, text_continue.box_size.x, text_continue.box_size.y, text_continue.color);

		// Blink the click to continue text
		pause_time_elapsed += CP_System_GetDt();
		if (pause_time_elapsed > pause_blink_speed) {
			DrawTextCentre(text_continue.text, text_continue.pos.x, text_continue.pos.y, text_continue.color, text_continue.size);

			if (pause_time_elapsed > pause_blink_speed * 2) {
				pause_time_elapsed = 0.0f;
			}
		}

		// If click, continue
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) {
			timer_paused = FALSE;

			if (tut_time_elapsed < time_section_1_end) {
				tut_time_elapsed = time_section_1_end;
			}
			else {
				tut_time_elapsed = time_section_2_end;
			}
		}
	}
	else {
		tut_time_elapsed += CP_System_GetDt();
	}
}

/*______________________________________________________________
@brief When a piece is played, end the tutorial by unsubscribing the render function from update.
*/
void TutorialExit(void) {
	// Hide the tutorial upon completion of the player's first turn
	UnsubscribeEvent(PLAYER_UPDATE, TutorialUpdate);
	UnsubscribeEvent(PLAYER_END, TutorialExit);
}

//______________________________________________________________
// Initialization functions

/*______________________________________________________________
@brief Loads the information for rendering text, like position, size, color into the TutText struct.
*/
TutText TutTextCreate(char* message, CP_Vector pos, CP_Color color, float size, CP_Vector box_bounds, CP_Vector box_size) {
	TutText new_text = { .text = message, .pos = pos, .color = color, .size = size, .box_bounds = box_bounds, .box_size = box_size };
	return new_text;
}

/*______________________________________________________________
@brief Loads the information for rendering arrows, like position, size, color into the TutArrow struct.
*/
TutArrow TutArrowCreate(CP_Vector pos_start, CP_Vector pos_end, CP_Color color, float thickness) {
	TutArrow new_arrow = { .pos_start = pos_start, .pos_end = pos_end, .color = color, .thickness = thickness };
	return new_arrow;
}

//______________________________________________________________
// Rendering functions

/*______________________________________________________________
@brief Renders the arrow based on the information in TutArrow.
*/
void DrawArrow(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness) {
	CP_Vector line = CP_Vector_Set(point_1_x - point_2_x, point_1_y - point_2_y);
	line = CP_Vector_Normalize(line);
	line = CP_Vector_Scale(line, arrow_head); // Second parameter is size of triangle at the end of the arrow

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

/*______________________________________________________________
@brief Renders the text based on the information in TutText.
*/
void DrawTextFull(char* text, float pos_x, float pos_y, CP_Color color, CP_TEXT_ALIGN_HORIZONTAL h_align, CP_TEXT_ALIGN_VERTICAL v_align, float size) {
	CP_Settings_TextAlignment(h_align, v_align);
	CP_Settings_TextSize(size);
	CP_Settings_Fill(color);
	CP_Font_DrawText(text, pos_x, pos_y);
}

/*______________________________________________________________
@brief Renders the text with its anchor point on the top left.
*/
void DrawTextTopLeft(char* text, float pos_x, float pos_y, CP_Color color, float size) {
	DrawTextFull(text, pos_x, pos_y, color, CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP, size);
}

/*______________________________________________________________
@brief Renders the text with its anchor point in the center.
*/
void DrawTextCentre(char* text, float pos_x, float pos_y, CP_Color color, float size) {
	DrawTextFull(text, pos_x, pos_y, color, CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE, size);
}

/*______________________________________________________________
@brief Renders the background box for the text
*/
void DrawTextBox(float pos_x, float pos_y, float size_x, float size_y, CP_Color color) {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(BLACK);
	CP_Settings_Stroke(color);
	CP_Settings_StrokeWeight(box_stroke);
	CP_Graphics_DrawRect(pos_x, pos_y, size_x, size_y);
}
