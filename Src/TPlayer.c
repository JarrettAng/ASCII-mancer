/*!
@file	  TPlayer.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include "TManager.h" 
#include "TPlayer.h" 

TetrisPiece hand[HAND_SIZE];
TetrisPiece peek_hand[PEEK_SIZE];

CP_Vector hand_top_left, peek_top_left;

#pragma region
void PlayPiece(int played_index);
#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the TPlayer.h)

/*______________________________________________________________
@brief Needs to be called at the start of the game level (and after the Tetris Pieces
       has been initialized) so that the player has pieces to play with.
*/
void TPlayerInit(void) {
    hand_top_left = CP_Vector_Set(CP_System_GetWindowWidth() / 2 - (HAND_SIZE * SHAPE_BOUNDS * PIECE_TILE_SIZE + HAND_SIZE / 2 * PIECE_SPACING) / 2, CP_System_GetWindowHeight() - EDGE_BUFFER);
    peek_top_left = CP_Vector_Set(hand_top_left.x + HAND_SIZE * SHAPE_BOUNDS * PIECE_TILE_SIZE + HAND_SIZE / 2 * PIECE_SPACING, hand_top_left.y);

    // First, fill the player's hand
    for (int index = 0; index < HAND_SIZE; ++index) {
        hand[index] = DrawFromBag();
    }

    // Second, fill the peek's queue (Basically the upcoming pieces)
    for (int index = 0; index < PEEK_SIZE; ++index) {
        peek_hand[index] = DrawFromBag();
    }
}

// JARRETT TODO: Finish pick up piece stub function
void PickUpPiece(void) {
    // JARRETT TODO: If piece is dropped onto the grid, get the corresponding pos and do something
    // PlayPiece();

    // JARRETT TODO: If piece is dropped onto hand, return it and carry on
}

// JARRETT TODO: Finish render hand stub function
void RenderHand(void) {
    
}

//______________________________________________________________
// Player interaction functions

/*______________________________________________________________
@brief When a Tetris Piece is dropped onto the grid, it has been played.

@param int - Which piece was played
*/
void PlayPiece(int played_index) {
    if (played_index == -1) return;

    // JARRETT TODO: Insert the interaction of the piece with the grid here

    // Remove the piece from the player's hand, and shift the pieces behind fowards
    for (int index = played_index; index < HAND_SIZE - 1; ++index) {
        hand[index] = hand[index + 1];
    }

    // Move the next piece from the peek queue into the hand
    hand[HAND_SIZE - 1] = peek_hand[0];

    // Shift the peek queue piece foward
    for (int index = 0; index < PEEK_SIZE - 1; ++index) {
        peek_hand[index] = peek_hand[index + 1];
    }

    // Draw another piece from the queue
    hand[PEEK_SIZE - 1] = DrawFromBag();
}
