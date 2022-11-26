#pragma once
/*!
@file	  GameOver.h
@author	  Justine Ilao (justine.c@digipen.edu)
@date     23/11/2022
@brief    This source file declares the GameOver states and all the function definitions to render all the
text in the Game Over screen
________________________________________________________________________________________________________*/

#include "cprocessing.h"
#define HEADER_FONT_SIZE 120.f
#define DEFAULT_FONT_SIZE 48.f


//------------------------------
// Placeholder Text Maker
//------------------------------

// Making the struct of text
typedef struct TextOnly {
	float font_size;
	float xpos;
	float ypos;
	CP_Color color;
	char* words;
} TextOnly;

//-------------------------------

//TextOnly CreateText(char* words, float font_size, CP_Color color, float xpos, float ypos);
/*----------------------------------------------------------------------------
@brief Renders only the text, using the data from TextOnly structs
@param The TextOnly struct to be rendered
------------------------------------------------------------------------------*/
void RenderGOText(TextOnly Rendered_Text);
/*----------------------------------------------------------------------------
@brief Initializes all the TextOnly structs
------------------------------------------------------------------------------*/
void InitializeAllText(void);
/*----------------------------------------------------------------------------
@brief Renders all the TextOnly structs and as well as the Wave number and Enemies
Killed number in the Game Over screen
------------------------------------------------------------------------------*/
void RenderAllText(void);

/*----------------------------------------------------------------------------
@brief Initializes the buttons like the back to main menu button in the game over
screen
------------------------------------------------------------------------------*/
void InitializeButtonsGameOverScreen(void);
/*----------------------------------------------------------------------------
@brief Sets the next game state to the main menu
------------------------------------------------------------------------------*/
void BackToMainMenu(void);

//-------------------------------

/*----------------------------------------------------------------------------
@brief GameOverInit state, initializes the font, the background and all the text
and buttons for the Game Over screen
------------------------------------------------------------------------------*/
void GameOverInit(void);
/*----------------------------------------------------------------------------
@brief GameOverUpdate state, all the buttons and text are rendered in this state
------------------------------------------------------------------------------*/
void GameOverUpdate(void);
/*----------------------------------------------------------------------------
@brief GameOverExit state, resets the game parameters, wave count and enemies killed
as well as clearing the UI elements in the GameOver state
------------------------------------------------------------------------------*/
void GameOverExit(void);
