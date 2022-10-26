/*!
@file	  TPiece.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     26/10/2022
@brief    This header file contains the information about Tetris pieces, like the types of all Tetris Pieces as well as the
		  actual Tetris Piece struct.
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "cprocessing.h" // For CP_Color

//______________________________________________________________
// All possible Tetris Piece types
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

//______________________________________________________________
// Tetris Piece 
#define SHAPE_BOUNDS 5 // The maximum size of pieces (e.g. if set to 5, maximum size of a piece is a 5x5 block)
typedef struct {
	PieceType type;

	int shape[SHAPE_BOUNDS][SHAPE_BOUNDS];
	int rotation;

	CP_Color color;
} TetrisPiece;

//______________________________________________________________
// All possible Tetris Piece shapes 

/* Preview: #
			#
			#
			# */
#define I_PIECE_SHAPE = "#\n#\n#\n#\0";

/* Preview: ##
			## */
#define O_PIECE_SHAPE = "##\n##\0";

/* Preview: ###
			 # */
#define T_PIECE_SHAPE = "###\n0#0\0";

/* Preview: ##
			 #
			 # */
#define L_PIECE_SHAPE = "##\n0#\n0#\0";

/* Preview: ##
			#
			# */
#define J_PIECE_SHAPE = "##\n#\n#\0";

/* Preview: ##
			 ## */
#define Z_PIECE_SHAPE = "##\n0##\0";

/* Preview:  ##
			## */
#define S_PIECE_SHAPE = "0##\n##\0";