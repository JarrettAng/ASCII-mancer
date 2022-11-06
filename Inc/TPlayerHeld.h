/*!
@file	  TPlayerHeld.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     05/11/2022
@brief    This header file
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "TPiece.h"

typedef struct {
    TetrisPiece const* piece;

    CP_Vector draw_pos;
    float x_screen_length, y_screen_length;
    CP_Vector center_offset;

    CP_Color color;
    CP_Color color_stroke;
} PlayerPieceHeld;

typedef struct {
    int grid_x, grid_y;
    int cell;
} PieceHeldCell;

typedef struct {
    PieceHeldCell grid[SHAPE_BOUNDS * SHAPE_BOUNDS];
} PieceHeldShape;

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    ROTATION_TYPE_LENGTH
} PieceOrientation;

/*______________________________________________________________
@brief Called by TPlayer during its initialization, this function will load
       the values needed for rendering and such.
*/
void TPlayerHeldInit(void);

/*______________________________________________________________
@brief Returns true if a piece is currently held by the player.
*/
_Bool IsPieceHeld(void);

/*______________________________________________________________
@brief Returns true if the piece to compare is the piece held, 
       if no piece is held it will return false.
*/
_Bool IsThisPieceHeld(TetrisPiece const* piece_to_compare);

/*______________________________________________________________
@brief Called by TPlayer when a click has been detected on one of the slots
       The information of the piece clicked will be passed through here.
*/
void NewPieceHeld(TetrisPiece const *new_piece);

/*______________________________________________________________
@brief Called by TPlayer during its process input, this function will 
       handle all input related to the piece held
*/
void TPlayerHeldProcessInput(void);

/*______________________________________________________________
@brief Render the piece held by the player, if any.
*/
void RenderPieceHeld(void);
