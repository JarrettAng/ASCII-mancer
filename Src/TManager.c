/*!
@file	  TManager.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    TManager's header file handles the interfacing of Tetris pieces and any other files in the project; 
          This source file
________________________________________________________________________________________________________*/

#include <stdlib.h> // For srand & rand
#include "TManager.h"
#include "ColorTable.h"

// Markers for which piece is the next piece in queue
int current_bag = 0;
int current_index = 0;

PieceType pieces_bag[TOTAL_BAGS][TOTAL_PIECES]; // The Tetris Pieces queue

#pragma region
void FillBag(int bag);
void ShuffleBag(int bag);
int RandInt(int min, int max);
TetrisPiece CreatePiece(PieceType type);
#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the TManager.h)

/*______________________________________________________________
@brief Please initialize this at the start of the game level so there'll actually be Tetris Pieces to use.
*/
void TManagerInit(int rand_seed) {
    srand(rand_seed);

    for (int index = 0; index < TOTAL_BAGS; ++index) {
        FillBag(index);
        ShuffleBag(index);
    }
}

/*______________________________________________________________
@brief Draws the next Tetris Piece from the queue, the queue will never run out of pieces.

@return TetrisPiece - The next Tetris Piece in queue.
*/
TetrisPiece DrawFromBag(void) {
    PieceType drawn_piece = pieces_bag[current_bag][current_index++];

    // If the queue is running low, shuffle in more pieces.
    if (current_index > TOTAL_PIECES - 1) {
        ShuffleBag(current_bag);
        current_bag = !current_bag;
        current_index = 0;
    }

    return CreatePiece(drawn_piece);
}

//______________________________________________________________
// Tetris Spawning functions

/*______________________________________________________________
@brief Fills the chosen bag with a copy of every Tetris Piece possible.

@param int - The index of the bag to fill, the bags are placed in an array.
*/
void FillBag(int bag) {
    int *current_bag = &pieces_bag[bag];
    for (int index = 0; index < TOTAL_PIECES; ++index) {
        current_bag[index] = index;
    }
}

/*______________________________________________________________
@brief Shuffles the chosen bag, the Fisher–Yates shuffle is used for the shuffling process.

@param int - The index of the bag to shuffle, the bags are placed in an array.
*/
void ShuffleBag(int bag) {
    int *current_bag = &pieces_bag[bag];
    for (int index = TOTAL_PIECES - 1; index > 0; --index) {
        int rand_index = RandInt(0, index);

        PieceType rand_piece = current_bag[rand_index];
        current_bag[rand_index] = current_bag[index];
        current_bag[index] = rand_piece;
    }
}

/*______________________________________________________________
@brief Returns a random int. Used by ShuffleBag to shuffle the pieces Fisher–Yates shuffle.

@param int - The smallest number (inclusive)
       int - The largest number (inclusive)
@return int - A random int between min and max
*/
int RandInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/*______________________________________________________________
@brief Creates a Tetris Piece based on the chosen type.

@param PieceType - What kind of piece should be spawned.
@return TetrisPiece - The new tetris piece, warranty not included.
*/
TetrisPiece CreatePiece(PieceType type) {
    TetrisPiece new_piece = { .color = WHITE, .rotation = 0, .type = type };
    
    // JARRETT TODO: actually add in the shape functionality later
    // new_piece.shape = I_PIECE_SHAPE;

    return new_piece;
}
