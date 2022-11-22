/*!
@file	  TPiece.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@date     17/11/2022
@brief    This header file contains the information about Tetris pieces, e.g. the types and shapes of all Tetris Pieces 
		  as well as the actual Tetris Piece struct.
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "cprocessing.h" // For CP_Color

/*______________________________________________________________
@brief All possible Tetris Piece types
*/
#define TOTAL_PIECES 7
typedef enum {
	I_PIECE,
	O_PIECE,
	T_PIECE,
	L_PIECE,
	J_PIECE,
	Z_PIECE,
	S_PIECE
} PieceType;

/*______________________________________________________________
@brief Tetris Piece information, like shape, length in terms of index and screen, etc.
*/
#define SHAPE_BOUNDS 6 // The maximum size of pieces (e.g. if set to 5, maximum size of a piece is a 5x5 block)
typedef struct {
	PieceType type;

	int shape[SHAPE_BOUNDS][SHAPE_BOUNDS];

	int x_length, y_length;
	float x_screen_length, y_screen_length;

	CP_Color color;
	CP_Color color_stroke;

	CP_Vector draw_pos; // The screen pos of the piece (Anchored on the top left)
} TetrisPiece;

//______________________________________________________________
// All possible Tetris Piece shapes 

/* Preview: #
			#
			#
			# */
#define I_PIECE_SHAPE "#\n#\n#\n#"

/* Preview: ##
			## */
#define O_PIECE_SHAPE "##\n##"

/* Preview: ###
			 # */
#define T_PIECE_SHAPE "###\n0#"

/* Preview: ##
			 #
			 # */
#define L_PIECE_SHAPE "##\n0#\n0#"

/* Preview: ##
			#
			# */
#define J_PIECE_SHAPE "##\n#\n#"

/* Preview: ##
			 ## */
#define Z_PIECE_SHAPE "##\n0##"

/* Preview:  ##
			## */
#define S_PIECE_SHAPE "0##\n##"
