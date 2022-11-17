/*!
@file	  TManager.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     17/11/2022
@brief    This header file handles the interfacing of Tetris pieces and any other files in the project,
          Any file not tetris-piece related that wants to use tetris pieces should include this header file instead.

          It contains a macros for the random seed, total number of pieces in play, on hand, and in the peek queue.
          It also contains 2 functions,

          TManagerInit - Needs to be initialized at the start of the game level so that the Tetris queue and player's hand can be filled.
          DrawFromBag - Draws the next piece from the Tetris queue into the player's hand.
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "TPiece.h" // For Tetris Piece information like shape & size

//______________________________________________________________
// Tetris Spawn settings

/* Each bag contains 1 of each piece, meaning if there are 2 bags and 7 total pieces, only 14 pieces will be in play at a time, 
   DO NOT set the sizes of the hand and peek to be larger than the size of all the bags */
#define TOTAL_BAGS 2 
#define DEFAULT_SPAWN_SEED 42069 // Useful for debugging by removing the randomness

//______________________________________________________________
// Player hand settings
#define HAND_SIZE 3             // How many pieces can the player hold at a time
#define PEEK_SIZE 2             // How many pieces can the player see coming next

/*______________________________________________________________
@brief TManager needs to be initialized at the start of the game level so that the Tetris queue and player's hand can be filled.

@param[in] rand_seed - Which seed to use to randomly spawn the Tetris pieces. (Use DEFAULT_SPAWN_SEED for debugging)
*/
void TManagerInit(int rand_seed);

/*______________________________________________________________
@brief Draws the next piece from the Tetris queue into the player's hand.

@return TetrisPiece - The next piece in queue.
*/
TetrisPiece DrawFromBag(void);
