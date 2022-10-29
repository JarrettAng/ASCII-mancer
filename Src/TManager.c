/*!
@file	  TManager.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    TManager's header file handles the interfacing of Tetris pieces and any other files in the project; 
          This source file contains the implementation of those functions.
________________________________________________________________________________________________________*/

#include <stdlib.h> // For srand & rand
#include "TManager.h"
#include "ColorTable.h"

// Markers for which piece is the next piece in queue
int current_bag;
int current_index;

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
    current_bag = current_index = 0;

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

    // If the queue is running low, swap bags and shuffle in more pieces.
    if (current_index > TOTAL_PIECES - 1) {
        ShuffleBag(current_bag);
        current_bag = (current_bag + 1) % TOTAL_BAGS;
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
    TetrisPiece new_piece = { .color = WHITE, .color_stroke = BLACK, .rotation = 0, .type = type, .x_length = 0, .y_length = 0, 
                              .x_screen_length = 0, .y_screen_length = 0, .shape = { 0 } , .draw_pos = { 0 } };
    
    // Set shape data for the new piece
    char *shape_data = "";
    switch (type) {
    case I_PIECE: shape_data = I_PIECE_SHAPE; break;
    case O_PIECE: shape_data = O_PIECE_SHAPE; break;
    case T_PIECE: shape_data = T_PIECE_SHAPE; break;
    case L_PIECE: shape_data = L_PIECE_SHAPE; break;
    case J_PIECE: shape_data = J_PIECE_SHAPE; break;
    case Z_PIECE: shape_data = Z_PIECE_SHAPE; break;
    case S_PIECE: shape_data = S_PIECE_SHAPE; break;
    }

    int index_x = 0, index_y = 0;
    char *current = shape_data;
    while (*current != '\0') {
        switch(*current) {
        case '#': new_piece.shape[index_x][index_y] = 1; ++index_x; break;
        case '\n': ++index_y;
                   new_piece.x_length = (new_piece.x_length > index_x) ? new_piece.x_length : index_x;
                   index_x = 0; break;
        default: ++index_x;
        }

        ++current;
    }

    // Update the length of the piece
    new_piece.x_length = (new_piece.x_length > index_x) ? new_piece.x_length : index_x;
    new_piece.y_length = index_y + 1;

    return new_piece;
}
