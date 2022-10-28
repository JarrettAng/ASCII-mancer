/*!
@file	  TPlayer.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    This header file
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

//______________________________________________________________
// Tetris Rendering settings
#define PIECE_TILE_SIZE 50
#define PIECE_SPACING 50
#define EDGE_BUFFER 50

/*______________________________________________________________
@brief Needs to be called at the start of the game level (and after the Tetris Pieces
       has been initialized) so that the player has pieces to play with.
*/
void TPlayerInit(void);
