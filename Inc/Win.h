#pragma once
#include "cprocessing.h"

#define WAVES_TO_WIN 30

/*----------------------------------------------------------------------------
@brief Initializes the buttons for the Win Screen
------------------------------------------------------------------------------*/
void InitializeButtonsWinScreen(void);

/*----------------------------------------------------------------------------
@brief Initializes the text for the Win Screen
------------------------------------------------------------------------------*/
void InitializeAllTextWin(void);

/*----------------------------------------------------------------------------
@brief Initializes the text and background for the title card
for the transition when winning the game
------------------------------------------------------------------------------*/
void WinTextInit(void);

/*----------------------------------------------------------------------------
@brief Renders all the text for the Win Screen as well as the tracked number for
the Waves and the amount of Enemies Killed
------------------------------------------------------------------------------*/
void RenderAllTextWin(void);

/*----------------------------------------------------------------------------
@brief Checks if the player has won the game
------------------------------------------------------------------------------*/
void CheckWinCondition(void);

/*----------------------------------------------------------------------------
@brief Renders the Win Animation
------------------------------------------------------------------------------*/
void WinAnimation(void);

/*----------------------------------------------------------------------------
@brief WinInit state, loads the background, font and all the texts and buttons
------------------------------------------------------------------------------*/
void WinInit(void);

/*----------------------------------------------------------------------------
@brief Updates the Win Screen, renders the background, font, all the texts and
buttons
------------------------------------------------------------------------------*/
void WinUpdate(void);

/*----------------------------------------------------------------------------
@brief WinExit state, reset the wave and the enemies killed counter and also frees
the UI elements like the buttons
------------------------------------------------------------------------------*/
void WinExit(void);
