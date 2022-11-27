/*!
@file	  gamelevel.h
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@author	  Justine Carlo Villa Ilao (justine.c@digipen.edu)
@author	  Muhammad Farhan Bin Ahmad (b.muhammadfarhan@digipen.edu)
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This header file contains the information about the gamelevel

@license  Copyright © 2022 DigiPen, All rights reserved.
_______________________________________________________________________*/

#pragma once // Only include this header file once

/*
@brief Initialises all the necessary game level functions
*/
void gameLevelInit(void);
/*____________________________________________________________________*/


/*
@brief Update loop of the game
*/
void gameLevelUpdate(void);
/*____________________________________________________________________*/


/*
@brief Called on exit for game.
*/
void gameLevelExit(void);
/*____________________________________________________________________*/


/*
@brief Displays the current wave to game level
*/
void ShowCurrentWave(void);
/*____________________________________________________________________*/


/*
@brief Test function for showing if game is in playtest mode or not.
*/
void ShowPlaytestMode(void);
/*____________________________________________________________________*/


/*
@brief Test function for showing total enemies killed
*/
void ShowTestEnemiesKilled(void);
/*____________________________________________________________________*/


/*
@brief Function to exit to main menu. (ESC to exit)
*/
void ExitToMainMenu(void);