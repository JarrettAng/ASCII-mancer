/*!
@file	  TPlayerHeld.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     05/11/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include "ColorTable.h"

#include "TPlayerHeld.h"

PlayerPieceHeld piece_held;

//______________________________________________________________
// All "public" functions (Basically those in the TPlayer.h)

/*______________________________________________________________
@brief Called by TPlayer during it's initialization, this function will load
	   the values needed for rendering and such
*/
void TPlayerHeldInit(void) {
	piece_held.piece = NULL;
	// TODO: LINK WITH ACTUAL GRID SETTINGS
	// Change the render color & size to match the grid
	piece_held.color = MENU_RED;
	piece_held.color_stroke = TRANSPERANT;
	piece_held.x_screen_length = 75.0f;
	piece_held.y_screen_length = 75.0f;
}

_Bool IsPieceHeld(void) {
	return piece_held.piece != NULL;
}

_Bool IsThisPieceHeld(TetrisPiece const *piece_to_compare) {
	return &piece_held.piece == &piece_to_compare;
}

void NewPieceHeld(TetrisPiece const *new_piece) {
	// We found the slot clicked! Set the flags to true
	piece_held.piece = new_piece;

	// Update the render offset
	piece_held.center_offset.x = (piece_held.piece->x_length / 2 + 0.5f) * piece_held.x_screen_length;
	piece_held.center_offset.y = (piece_held.piece->y_length / 2 + 0.5f) * piece_held.y_screen_length;
}

void RenderPieceHeld(void) {
	if (!IsPieceHeld()) return;

	CP_Vector mouse_pos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

	// The player should hold the center of the piece, if the piece has an even length (e.g. 2x2) bias towards the end
	piece_held.draw_pos.x = mouse_pos.x - piece_held.center_offset.x;
	piece_held.draw_pos.y = mouse_pos.y - piece_held.center_offset.y;

	// Settings for tile rendering
	CP_Settings_Fill(piece_held.color);
	CP_Settings_Stroke(piece_held.color_stroke);
	// Render each tile in the Tetris Piece
	for (int index_x = 0; index_x < SHAPE_BOUNDS; ++index_x) {
		for (int index_y = 0; index_y < SHAPE_BOUNDS; ++index_y) {
			if (piece_held.piece->shape[index_x][index_y]) {
				CP_Graphics_DrawRect(piece_held.draw_pos.x + index_x * piece_held.x_screen_length, piece_held.draw_pos.y + index_y * piece_held.y_screen_length, piece_held.x_screen_length, piece_held.y_screen_length);
			}
		}
	}
}

void PieceHeldReleased(void) {
	piece_held.piece = NULL;
}
