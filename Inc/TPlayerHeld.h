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

/*______________________________________________________________
@brief Called by TPlayer during it's initialization, this function will load
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
@brief Render the piece held by the player, if any.
*/
void RenderPieceHeld(void);

/*______________________________________________________________
@brief Removes information on piece held once the player has released.
*/
void PieceHeldReleased(void);
