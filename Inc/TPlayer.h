/*!
@file	  TPlayer.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    This header file
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "TPiece.h"

typedef struct {
    CP_Vector pos;
    CP_Vector icon_pos;
    TetrisPiece piece;
} PlayerHandSlot;

//______________________________________________________________
// Tetris Rendering settings
float hand_total_height;
float hand_bottom_buffer;

float hand_total_length;
float hand_left_buffer;
float hand_left_extra_buffer;
float hand_slot_length;
float hand_slot_spacing;

float peek_total_length;
float peek_right_buffer;
float peek_slot_length;
float peek_slot_spacing;

float hand_tile_length;
float hand_tile_stroke;

float peek_tile_length;
float peek_tile_stroke;

float text_peek_size;
float text_icon_size;

/*______________________________________________________________
@brief Needs to be called at the start of the game level (and after the Tetris Pieces
       has been initialized) so that the player has pieces to play with.
*/
void TPlayerInit(void);

void TPlayerProcessInput(void);

void RenderHand(void);

/*______________________________________________________________
@brief When a Tetris Piece is dropped onto the grid, update the player's hand
*/
void RemovePieceHeldFromHand(void);
