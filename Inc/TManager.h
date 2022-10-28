/*!
@file	  TManager.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    // This header file handles the interfacing of Tetris pieces and any other files in the project //
          Any file not tetris-piece related that wants to use tetris pieces will include this header file and none of the other tetris-piece files.
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "TPiece.h"

//______________________________________________________________
// Tetris Spawn settings
#define TOTAL_BAGS 2
#define DEFAULT_SPAWN_SEED 42069 // Useful for debugging by removing the randomness

//______________________________________________________________
// Player hand settings
#define HAND_SIZE 3
#define PEEK_SIZE 2

/*______________________________________________________________
@brief TManager needs to be initialized at the start of the game level so that 
       the Tetris queue and player's hand can be filled.

@param int - Which seed to use to randomly spawn the Tetris pieces. (Use DEFAULT_SPAWN_SEED for debugging)
*/
void TManagerInit(int rand_seed);

/*______________________________________________________________
@brief Draws the next piece from the Tetris queue into the player's hand.

@return TetrisPiece - The next piece in queue.
*/
TetrisPiece DrawFromBag(void);
