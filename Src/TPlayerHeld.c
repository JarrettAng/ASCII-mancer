/*!
@file	  TPlayerHeld.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     22/11/2022
@brief    This source file handles everything about the current piece held by the player, if any.
		  It contains matrix arrays to store the shape of the held piece, and for rotations. A pointer for information on
		  render and 13 functions,

		  TPlayerHeldInit - Called by TPlayer during it's initialization, this function will load all the values needed.
		  LoadIconImages - Called by TPlayer during its initialization, this function loads the attack and defend icons.

		  IsPieceHeld - Checks if any piece is currently held by the player, and returns true/false.
		  IsThisPieceHeld - Called by TPlayer when rendering, returns true if the piece to compare is the piece held.
		  NewPieceHeld - Called by TPlayer when a click has been detected on one of the slots.
		  TPlayerHeldProcessInput - Called by TPlayer during its process input, handles all input related to the piece held.
		  RenderPieceHeld - Render the piece held by the player, if any.

		  PieceHeldPlayed - When a Tetris Piece is dropped onto the grid, it has been played.
		  PieceHeldRotateRight - Applies a 90 degree rotation to the right on the shape array (matrix) for the piece 

		  ShapeToIndex - Converts a 2D array index into a 1D array index in shape array
		  GridToIndex - Using a system where 0 is the origin (e.g. -3 to 3) covert it to index in shape array
		  IndexToShapeX - Converts a 1D array index into the x index in the 2D shape array
		  IndexToShapeY - Converts a 1D array index into the y index in the 2D shape array
________________________________________________________________________________________________________*/

#include "ColorTable.h"		// For tetris colors
#include "Grid.h"			// For grid information
#include "WaveSystem.h"		// For enemy damaging on grid
#include "Particles.h"		// For particles on play
#include "Screenshake.h"	// For screenshake on play
#include "SoundManager.h"	// For sounds when pieces are played

#include "TPlayer.h"		// For removing piece from player's hand
#include "TPlayerHeld.h"	
#include "GameLoop.h"		// For turn swapping

PlayerPieceHeld piece_held; // Information on the piece held

// Icon information
CP_Vector icon_pos;		// Each cell will have a attack/defend icon at the top left
CP_Vector icon_size;	// The size of the icon for rendering
float piece_stroke;		// How thick the stroke of each cell should be

// Shape information
PieceHeldShape piece_held_shapeA;			// Two matrices for rotation, ping-ponging between the two
PieceHeldShape piece_held_shapeB;
PieceHeldShape *piece_held_shapeCurrent;	// Pointer to the current matrix used out of the two
int piece_held_shape_centre;				// The index that is the center of the array

// Transformation matrices
int left_rotation[2][2] = { 0, -1, 1, 0 };	// 90 degrees rotation left
int right_rotation[2][2] = { 0, 1, -1, 0 };	// 90 degrees rotation right
PieceOrientation current_rotation;			// Which of the 4 direction is the piece currenting facing

_Bool in_playing_area;	// Render differently if it is/isn't in the playing area (grid)

#pragma region
void PieceHeldPlayed(int grid_x, int grid_y);
void PieceHeldRotateRight(void);
int ShapeToIndex(int shape_x, int shape_y);
int GridToIndex(int grid_x, int grid_y);
int IndexToShapeX(int index);
int IndexToShapeY(int index);
#pragma endregion Forward Declarations

CP_Image attack_icon, shield_icon;

//______________________________________________________________
// All "public" functions (Basically those in the TPlayer.h)

/*______________________________________________________________
@brief Called by TPlayer during it's initialization, this function will load the values needed for rendering and such.
*/
void TPlayerHeldInit(void) {
	piece_held.piece = NULL;

	// Initialize Piece on grid rendering information & the render color & size to match the grid
	piece_held.color = TETRIS_HOVER_COLOR;
	piece_held.color_stroke = TETRIS_COLOR;
	piece_held.x_screen_length = GetCellSize();
	piece_held.y_screen_length = GetCellSize();

	// Initialize shape matrices for the piece held
	piece_held_shape_centre = SHAPE_BOUNDS / 2;
	piece_held_shapeCurrent = &piece_held_shapeA;
	for (int index = 0; index < SHAPE_BOUNDS * SHAPE_BOUNDS; ++index) {
		piece_held_shapeCurrent->grid[index].grid_x = IndexToShapeX(index);
		piece_held_shapeCurrent->grid[index].grid_y = IndexToShapeY(index);
		piece_held_shapeCurrent->grid[index].cell = 0;
	}
	piece_held_shapeCurrent = &piece_held_shapeB;
	for (int index = 0; index < SHAPE_BOUNDS * SHAPE_BOUNDS; ++index) {
		piece_held_shapeCurrent->grid[index].grid_x = IndexToShapeX(index);
		piece_held_shapeCurrent->grid[index].grid_y = IndexToShapeY(index);
		piece_held_shapeCurrent->grid[index].cell = 0;
	}

	// Initialize the icon pos offset, the top left corner of each cell
	icon_pos.x = -GetCellSize() / 2.0f;
	icon_pos.y = -GetCellSize() / 2.0f;

	// The size of the icon is 60% of cell length
	icon_size.x = GetCellSize() * 0.6f;
	icon_size.y = GetCellSize() * 0.6f;

	// Update the stroke to be 5% of the cell size
	piece_stroke = GetCellSize() * 0.05f;
}

/*______________________________________________________________
@brief Called by TPlayer during its initialization, this function loads the attack and defend icons.

@param[in] attack - Attack icon (For pieces that do damage)
@param[in] shield - Defend icon (For pieces that build walls)
*/
void LoadIconImages(CP_Image attack, CP_Image shield) {
	attack_icon = attack;
	shield_icon = shield;
}

/*______________________________________________________________
@brief Checks if any piece is currently held by the player, and returns true/false.

@return _Bool - Returns true if any piece is currently held by the player.
*/
_Bool IsPieceHeld(void) {
	return piece_held.piece != NULL;
}

/*______________________________________________________________
@brief Called by TPlayer when rendering, returns true if the piece to compare is the piece held,
	   if no piece is held it will also return false.

@param[in] piece_to_compare - The piece in the player's hand to compare with

@return _Bool - Returns true if the piece compared is the same as the piece held.
*/
_Bool IsThisPieceHeld(TetrisPiece const *piece_to_compare) {
	return piece_held.piece == piece_to_compare;
}

/*______________________________________________________________
@brief Called by TPlayer when a click has been detected on one of the slots.
	   The information of the piece clicked will be passed through here.

@param[in] new_piece - Pointer to the information of the new piece held
@param[in] slot_index - Which slot was this piece taken from?
*/
void NewPieceHeld(TetrisPiece const *new_piece, int slot_index) {
	// We found the slot clicked! Set the flags to true
	piece_held.piece = new_piece;

	// Update the shape matrix for the piece held
	// First clear the shape array
	for (int index = 0; index < SHAPE_BOUNDS * SHAPE_BOUNDS; ++index) {
		piece_held_shapeCurrent->grid[index].cell = 0;
	}

	// Start with the top left corner
	int index;
	int start_x = piece_held_shape_centre - new_piece->x_length / 2;
	int start_y = piece_held_shape_centre + new_piece->y_length / 2;
	for (int index_x = start_x, piece_x = 0; index_x < start_x + new_piece->x_length; ++index_x, ++piece_x) {
		for (int index_y = start_y, piece_y = new_piece->y_length - 1; index_y > start_y - new_piece->y_length; --index_y, --piece_y) {
			index = ShapeToIndex(index_x, index_y);
			piece_held_shapeCurrent->grid[index].cell = piece_held.piece->shape[piece_x][piece_y];
		}
	}

	// Update the render offset
	piece_held.center_offset.x = (piece_held_shape_centre + 0.5f) * piece_held.x_screen_length;
	piece_held.center_offset.y = (piece_held_shape_centre + 0.5f) * piece_held.y_screen_length;
	current_rotation = UP;

	// Update the slot index to match
	piece_held.slot_index = slot_index;
}

/*______________________________________________________________
@brief Called by TPlayer during its process input, this function will handle all input related to the piece held.
*/
void TPlayerHeldProcessInput(void) {
	if (!IsPieceHeld()) return;

	// Update piece position based on where it current is.
	CP_Vector mouse_pos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

	// If the piece is within the grid, snap the piece to the grid
	// JARRETT TODO: Add the checks for all the sides of the piece
	if (in_playing_area = IsInPlayingArea( mouse_pos.x, mouse_pos.y)) {
		mouse_pos.x = GridXToPosX(PosXToGridX(mouse_pos.x));
		mouse_pos.y = GridYToPosY(PosYToGridY(mouse_pos.y));
	}

	// The player should hold the center of the piece, if the piece has an even length (e.g. 2x2) bias towards the end
	piece_held.draw_pos.x = mouse_pos.x - piece_held.center_offset.x;
	piece_held.draw_pos.y = mouse_pos.y - piece_held.center_offset.y;

	// When the player rotates/ right clicks
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_2)) {
		PieceHeldRotateRight();
	}

	// When the player lets go of a click
	if (CP_Input_MouseReleased(MOUSE_BUTTON_1)) {
		// If the piece is in the playing area, play it, otherwise it will just return to hand
		if (in_playing_area) {
			PieceHeldPlayed(PosXToGridX(mouse_pos.x), PosYToGridY(mouse_pos.y)); // Do damage and remove from hand
		}

		piece_held.piece = NULL; // Removes information on piece held once the player has released.
	}
}

/*______________________________________________________________
@brief Render the piece held by the player, if any.
*/
void RenderPieceHeld(void) {
	if (!IsPieceHeld()) return;

	// Settings for tile rendering
	CP_Settings_Stroke(piece_held.color_stroke);

	// Color whole piece red if not in grid
	_Bool hand_in_grid = IsInPlayingArea(CP_Input_GetMouseX(), CP_Input_GetMouseY());

	// Render each tile in the Tetris Piece
	CP_Vector current_pos;
	PieceHeldCell *current;
	for (int index = 0; index < SHAPE_BOUNDS * SHAPE_BOUNDS; ++index) {
		current = &piece_held_shapeCurrent->grid[index];
		if (current->cell) {
			current_pos.x = piece_held.draw_pos.x + current->grid_x * piece_held.x_screen_length;
			current_pos.y = piece_held.draw_pos.y + current->grid_y * piece_held.y_screen_length;

			// Set the stroke of the cell
			CP_Settings_StrokeWeight(piece_stroke);

			_Bool can_place = FALSE;
			// Color setting, red if piece is outside of grid, or invalid placement
			if (hand_in_grid && IsInPlayingArea(current_pos.x + piece_held.x_screen_length / 2.0f, current_pos.y + piece_held.y_screen_length / 2.0f)) {
				// If the cell is hovering over a live enemy in the grid cell
				if (HasLiveEnemyInCell(PosXToGridX(current_pos.x + piece_held.x_screen_length / 2.0f), PosYToGridY(current_pos.y + piece_held.y_screen_length / 2.0f))) {
					// If it is a shield, it is an invalid placement
					if (piece_held.slot_index == 0) {
						can_place = FALSE;
					}
					else { // Else, check if the entity is actually a wall and update accordingly
						EnemyInfo* enemy = GetAliveEnemyFromGrid(PosXToGridX(current_pos.x + piece_held.x_screen_length / 2.0f), PosYToGridY(current_pos.y + piece_held.y_screen_length / 2.0f));
						if (enemy->type != WALL) {
							can_place = TRUE;
						}
						else {
							can_place = FALSE;
						}
					}
				}
				else { // If the cell is not hovering over a live enemy,
					if (piece_held.slot_index == 0) { // If it is a shield, it is a valid placement
						can_place = TRUE;
					}
					else { // Otherwise, (for attacks) it is not a valid placement
						can_place = FALSE;
					}
				}
			}
			else { // If the piece is not even in the playing area, draw invalid placement
				can_place = FALSE;
			}

			// If the placement of the cell is valid, draw a light white square with the appropriate icon
			if (can_place) {
				CP_Settings_Stroke(piece_held.color_stroke);
				CP_Settings_Fill(piece_held.color);
				CP_Graphics_DrawRect(current_pos.x, current_pos.y, piece_held.x_screen_length, piece_held.y_screen_length);

				if (piece_held.slot_index == 0) {
					CP_Image_Draw(shield_icon, current_pos.x - icon_pos.x, current_pos.y - icon_pos.y, icon_size.x, icon_size.x, 255);
				}
				else {
					CP_Image_Draw(attack_icon, current_pos.x - icon_pos.x, current_pos.y - icon_pos.y, icon_size.x, icon_size.x, 255);
				}
			} // If the cell is not in the playing area, draw a red square
			else if (!hand_in_grid) {
				CP_Settings_Stroke(TETRIS_HOVER_RED_COLOR);
				CP_Settings_Fill(TETRIS_HOVER_RED_COLOR);
				CP_Graphics_DrawRect(current_pos.x, current_pos.y, piece_held.x_screen_length, piece_held.y_screen_length);
			}
			else { // If it is in the playing area, but the placement of the cell is not valid, draw a greyed out square
				CP_Settings_Stroke(TETRIS_HOVER_GREY_COLOR);
				CP_Settings_Fill(TETRIS_HOVER_GREY_COLOR);
				CP_Graphics_DrawRect(current_pos.x, current_pos.y, piece_held.x_screen_length, piece_held.y_screen_length);
			}
		}
	}
}

//______________________________________________________________
// Player interaction functions

/*______________________________________________________________
@brief When a Tetris Piece is dropped onto the grid, it has been played.
*/
void PieceHeldPlayed(int mouse_x, int mouse_y) {
	// Do damage to zombies covered by the piece
	PieceHeldCell* current = &piece_held_shapeCurrent->grid[0];
	for (int index = 0; index < SHAPE_BOUNDS * SHAPE_BOUNDS; ++index) {
		if (current[index].cell) {

			int grid_x = mouse_x + IndexToShapeX(index) - piece_held_shape_centre;
			int grid_y = mouse_y + IndexToShapeY(index) - piece_held_shape_centre;

			// Adjust grid positions for rotations (because pieces aren't symmetrical, they rotate unbalanced)
			switch (current_rotation) {
			case RIGHT: ++grid_x; break;
			case DOWN: ++grid_x; ++grid_y; break;
			case LEFT: ++grid_y; break;
			}

			// If it is outside the grid, continue instead
			if (IsIndexInPlayingArea(grid_x, grid_y)) {
				// Add a explosion effect particles on hit
				piece_held.slot_index == 0 ? 
				RadialParticleColor(GridXToPosX(grid_x), GridYToPosY(grid_y), 5, 1.5f,TETRIS_ICON_WALL_COLOR2):
				RadialParticleRGB(GridXToPosX(grid_x), GridYToPosY(grid_y), 5, 1.5f);
				// Depending on type, build a wall or send damage
				piece_held.slot_index == 0 ? CreateWall(grid_x, grid_y) : SendDamage(grid_x, grid_y, 1);
				
			}
		}
	}

	// Screen the shake
	trauma += 0.35f;

	// Play tetris piece sound
	piece_held.slot_index == 0 ? PlaySound(THUD, CP_SOUND_GROUP_SFX) : PlaySoundEx(TETROMINOEXPLODE, CP_SOUND_GROUP_SFX);

	// Swap to the zombie's turn
	GameLoopSwitch(TURN_ZOMBIE);

	RemovePieceHeldFromHand();
}

/*______________________________________________________________
@brief Applies a 90 degree rotation to the right on the shape array (matrix) for the piece 
*/
void PieceHeldRotateRight(void) {
	PieceHeldShape* previous = piece_held_shapeCurrent;
	piece_held_shapeCurrent = (piece_held_shapeCurrent == &piece_held_shapeA) ? &piece_held_shapeB : &piece_held_shapeA;

	int new_x, new_y, old_x, old_y;
	for (int index = 0; index < SHAPE_BOUNDS * SHAPE_BOUNDS; ++index) {
		// The shape index goes from 0 to 5 (Assuming length of 6) but we need it to go from -2 to 3, so we convert it.
		old_x = previous->grid[index].grid_x - piece_held_shape_centre;
		old_y = previous->grid[index].grid_y + piece_held_shape_centre - previous->grid[index].grid_y * 2;
		new_x = left_rotation[0][0] * old_x + left_rotation[1][0] * old_y;
		new_y = left_rotation[0][1] * old_x + left_rotation[1][1] * old_y;

		// In order to store it back into the array, we need to convert the -2 to 3 and to 0 to 5.
		piece_held_shapeCurrent->grid[GridToIndex(new_x, new_y)].cell = previous->grid[index].cell;
	}

	current_rotation = (current_rotation + 1) % ROTATION_TYPE_LENGTH;
	// Update the draw_pos to be centred on the new piece
	switch (current_rotation) {
	case UP:
		piece_held.center_offset.x = (piece_held_shape_centre + 0.5f) * piece_held.x_screen_length;
		piece_held.center_offset.y = (piece_held_shape_centre + 0.5f) * piece_held.y_screen_length;
		break;
	case RIGHT:
		piece_held.center_offset.x = (piece_held_shape_centre - 0.5f) * piece_held.x_screen_length;
		piece_held.center_offset.y = (piece_held_shape_centre + 0.5f) * piece_held.y_screen_length;
		break;
	case DOWN:
		piece_held.center_offset.x = (piece_held_shape_centre - 0.5f) * piece_held.x_screen_length;
		piece_held.center_offset.y = (piece_held_shape_centre - 0.5f) * piece_held.y_screen_length;
		break;
	case LEFT:
		piece_held.center_offset.x = (piece_held_shape_centre + 0.5f) * piece_held.x_screen_length;
		piece_held.center_offset.y = (piece_held_shape_centre - 0.5f) * piece_held.y_screen_length;
		break;
	}
}

//______________________________________________________________
// Piece rotating functions

/*______________________________________________________________
@brief Converts a 2D array index into a 1D array index in shape array
*/
int ShapeToIndex(int shape_x, int shape_y) {
	return shape_x + shape_y * SHAPE_BOUNDS;
}

/*______________________________________________________________
@brief Using a system where 0 is the origin (e.g. -3 to 3) covert it to index in shape array
*/
int GridToIndex(int grid_x, int grid_y) {
	int shape_x = grid_x + piece_held_shape_centre - 1;
	int shape_y = piece_held_shape_centre - (piece_held_shape_centre - (piece_held_shape_centre - grid_y));
	return ShapeToIndex(shape_x, shape_y);
}

/*______________________________________________________________
@brief Converts a 1D array index into the x index in the 2D shape array
*/
int IndexToShapeX(int index) {
	return index % SHAPE_BOUNDS;
}

/*______________________________________________________________
@brief Converts a 1D array index into the y index in the 2D shape array
*/
int IndexToShapeY(int index) {
	return index / SHAPE_BOUNDS;
}
