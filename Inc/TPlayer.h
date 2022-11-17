/*!
@file	  TPlayer.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     17/11/2022
@brief    This header file contains a struct used to store positions and piece information (like shape) for rendering,
          many variables for position scaling based on screen size, hand size, and peek size. As well as 5 functions,

          TPlayerInit - Needs to be called at the start of the game level so that the player has pieces to play with.

          TPlayerProcessInput - Needs to be called in update, checks for player clicks, updates TPlayerHeld to render the held piece accordingly.
          RenderHand - Needs to be called in update, renders everything in the player's hand, all the slots, pieces, text, etc.
          RemovePieceHeldFromHand - When a Tetris Piece is dropped onto the grid, remove it from the player's hand

          FreeIconImages - Needs to be called on the exit of game level, frees the icons for the attack and defend pieces
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "TPiece.h" // For Tetris Piece information like shape & size

/*______________________________________________________________
@brief Struct containing positions for rendering as well as piece information, like shape and size
*/
typedef struct {
    CP_Vector pos;
    CP_Vector icon_pos;
    TetrisPiece piece;
} PlayerHandSlot;

//______________________________________________________________
// Tetris Rendering settings
float hand_total_height;        // Upper bound of the hand panel
float hand_bottom_buffer;       // Lower bound of the hand panel

float hand_total_length;        // Various positions of the hand queue (for rendering)
float hand_left_buffer;
float hand_left_extra_buffer;
float hand_slot_length;
float hand_slot_spacing;

float peek_total_length;        // Various positions of the peek queue (for rendering)
float peek_right_buffer;
float peek_slot_length;
float peek_slot_spacing;

float hand_tile_length;         // The size of each square in a piece in the hand queue
float hand_tile_stroke;

float peek_tile_length;         // The size of each square in a piece in the peek queue
float peek_tile_stroke;

float text_peek_size;           // The size of the "< NEXT <" text
CP_Vector text_icon_size;

/*______________________________________________________________
@brief Needs to be called at the start of the game level (and after the Tetris Pieces
       has been initialized) so that the player has pieces to play with.
*/
void TPlayerInit(void);

/*______________________________________________________________
@brief Needs to be called in update, checks for player clicks, if the player clicks on a piece,
       it updates TPlayerHeld to render the held piece accordingly.
*/
void TPlayerProcessInput(void);

/*______________________________________________________________
@brief Needs to be called in update, renders everything in the player's hand, all the slots, pieces, text, etc.
*/
void RenderHand(void);

/*______________________________________________________________
@brief When a Tetris Piece is dropped onto the grid, remove it from the player's hand
*/
void RemovePieceHeldFromHand(void);

/*______________________________________________________________
@brief Needs to be called on the exit of game level, frees the icons for the attack and defend pieces
*/
void FreeIconImages(void);
