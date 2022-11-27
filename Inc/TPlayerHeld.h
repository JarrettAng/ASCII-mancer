/*!
@file	  TPlayerHeld.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@date     17/11/2022
@brief    This header file contains 3 structs used to store information about rotations and rendering,
          an enum to identify the current piece's rotation and 7 functions,

          TPlayerHeldInit - Called by TPlayer during its initialization, loads the values needed for rendering and such.
          LoadIconImages - Called by TPlayer during its initialization, loads the attack and defend icons.

          IsPieceHeld - Returns true if any piece is currently held by the player.
          IsThisPieceHeld - Returns true if the piece to compare is the piece held, if no piece is held it will return false.
          NewPieceHeld - Called by TPlayer when a click has been detected on one of the slots.

          TPlayerHeldProcessInput - Called by TPlayer during its process input, handles all input related to the piece held
          RenderPieceHeld - Render the piece held by the player, if any.

@license  Copyright © 2022 DigiPen, All rights reserved.
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "TPiece.h" // For Tetris Piece information like shape & size

/*______________________________________________________________
@brief Information about piece held by the player, like color, size, etc.
*/
typedef struct{
    TetrisPiece const* piece;

    CP_Vector draw_pos;
    float x_screen_length, y_screen_length;
    CP_Vector center_offset;

    CP_Color color;
    CP_Color color_stroke;

    int slot_index;
} PlayerPieceHeld;

/*______________________________________________________________
@brief Each cell in the piece's shape holds an index based on the origin (middle of array).
*/
typedef struct{
    int grid_x, grid_y;
    int cell;
} PieceHeldCell;

/*______________________________________________________________
@brief Holds all the cells in a SHAPE_BOUNDS by SHAPE_BOUNDS sized grid, used for rotation and rendering.
*/
typedef struct{
    PieceHeldCell grid[SHAPE_BOUNDS * SHAPE_BOUNDS];
} PieceHeldShape;

/*______________________________________________________________
@brief Indicators to tell which orientation the Tetris piece is in right now.
*/
typedef enum{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    ROTATION_TYPE_LENGTH
} PieceOrientation;

/*______________________________________________________________
@brief Called by TPlayer during its initialization, this function will load the values needed for rendering and such.
*/
void TPlayerHeldInit(void);

/*______________________________________________________________
@brief Called by TPlayer during its initialization, this function loads the attack and defend icons.

@param[in] attack - Attack icon (For pieces that do damage)
@param[in] shield - Defend icon (For pieces that build walls)
*/
void LoadIconImages(CP_Image attack, CP_Image shield);

/*______________________________________________________________
@brief Checks if any piece is currently held by the player, and returns true/false.

@return _Bool - Returns true if any piece is currently held by the player.
*/
_Bool IsPieceHeld(void);

/*______________________________________________________________
@brief Called by TPlayer when rendering, returns true if the piece to compare is the piece held,
       if no piece is held it will also return false.

@param[in] piece_to_compare - The piece in the player's hand to compare with

@return _Bool - Returns true if the piece compared is the same as the piece held.
*/
_Bool IsThisPieceHeld(TetrisPiece const* piece_to_compare);

/*______________________________________________________________
@brief Called by TPlayer when a click has been detected on one of the slots.
       The information of the piece clicked will be passed through here.

@param[in] new_piece - Pointer to the information of the new piece held
@param[in] slot_index - Which slot was this piece taken from?
*/
void NewPieceHeld(TetrisPiece const *new_piece, int slot_index);

/*______________________________________________________________
@brief Called by TPlayer during its process input, this function will 
       handle all input related to the piece held
*/
void TPlayerHeldProcessInput(void);

/*______________________________________________________________
@brief Render the piece held by the player, if any.
*/
void RenderPieceHeld(void);
