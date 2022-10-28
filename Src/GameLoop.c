/*!
@file	  GameLoop.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     27/10/2022
@brief    This source file 
________________________________________________________________________________________________________*/

#include "GameLoop.h"

TurnType current_turn;

#pragma region
void PlayerTurn(void);
void ZombieTurn(void);
#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the GameLoop.h)

/*______________________________________________________________
@brief Needs to be called at the start of the game level to handle the turns between
	   the player and the zombies
*/
void GameLoopInit(void) {
	// JARRETT TODO: First init the Tetris Pieces and then the player's hand

}

void GameLoopUpdate(void) {
	switch (current_turn) {
	case TURN_PLAYER: PlayerTurn(); break;
	case TURN_ZOMBIE: ZombieTurn(); break;
	}
}

//______________________________________________________________
// Game turn functions

void SetTurn(TurnType next) {
	current_turn = next;
}

void PlayerTurn(void) {
	// JARRETT TODO: Check input
}

void ZombieTurn(void) {

}
