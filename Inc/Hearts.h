#pragma once
/*!
@file	  GameOver.c
@author	  Justine Ilao (justine.c@digipen.edu)
@date     26/11/2022
@brief    This source file declares the GameOver states and all the function definitions to render all the
text in the Game Over screen
________________________________________________________________________________________________________*/
#define MAX_HEART_COUNT 3
#define IMAGE_HEART_LENGTH 263
#define HEART_SPACING 0.35f
#define HEART_SIZE 4.f
#define HEART_ALPHA 255
#define HEART_ROTATION 0.f

typedef struct HeartContainer {
	int heartAlive;
	float xpos;
	float ypos;
	float size;
	int alpha;
	float rotation;
}HeartContainer;

typedef struct TextOnlyHearts {
	float font_size;
	float xpos;
	float ypos;
	CP_Color color;
	char* words;
} TextOnlyHearts;

typedef struct RectHearts {
	float xpos;
	float ypos;
	float height;
	float width;
} RectHearts;

/*----------------------------------------------------------------------------
@brief Initializes the life image load, and fills in the HeartContainer struct
for the data of each heart to be initialized
------------------------------------------------------------------------------*/
void InitializeLife(void);

/*----------------------------------------------------------------------------
@brief Initializes the You Lose rendering and transistions to the Game Over screen
after a few seconds
------------------------------------------------------------------------------*/
void YouLoseTitleRender(void);

/*----------------------------------------------------------------------------
@brief Function to allow for the gaining of life
------------------------------------------------------------------------------*/
void GainLife(int gain_life);

/*----------------------------------------------------------------------------
@brief Function to allow for the losing of life
------------------------------------------------------------------------------*/
void LoseLife(int lose_life);

/*----------------------------------------------------------------------------
@brief Initializes the You Lose rendering and transistions to the Game Over screen
after a few seconds
------------------------------------------------------------------------------*/
void DrawLife(void);

/*----------------------------------------------------------------------------
@brief Checks if there are zero hearts, then if there are no hearts left for the player,
play the Game Over sound
------------------------------------------------------------------------------*/
int CheckLoseCondition(void);

/*----------------------------------------------------------------------------
@brief Renders the heart text
------------------------------------------------------------------------------*/
void RenderHeartText(void);

/*----------------------------------------------------------------------------
@brief Function for Bezier Curve
@param Place the tick time here, to have a smoother animation
------------------------------------------------------------------------------*/
float BezierCurve(float);

/*----------------------------------------------------------------------------
@brief Animates the hearts to go up and down
------------------------------------------------------------------------------*/
void AnimationLife(void);

/*----------------------------------------------------------------------------
@brief Updates the life to check for the lose condition, render the hearts and the
heart text and if the hearts are 0, start the You Lose render
------------------------------------------------------------------------------*/
void UpdateLife(void);

/*----------------------------------------------------------------------------
@brief Clear the heart image that was loaded previously
------------------------------------------------------------------------------*/
void ClearHearts(void);
