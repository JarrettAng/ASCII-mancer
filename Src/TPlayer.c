/*!
@file	  TPlayer.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    This source file
________________________________________________________________________________________________________*/

#include "TManager.h" 

TetrisPiece hand[HAND_SIZE];
TetrisPiece peek_hand[PEEK_SIZE];

#pragma region
void PlayPiece(int played_index);
#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the TPlayer.h)

/*______________________________________________________________
@brief Needs to be called at the start of the game level (and after the Tetris Pieces
       has been initialized) so that the player has pieces to play with.
*/
void FillHand(void) {
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
