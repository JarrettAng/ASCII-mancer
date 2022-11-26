#pragma once
/*!
@file	  Wizard.h
@author	  Justine Ilao (justine.c@digipen.edu)
@date     26/11/2022
@brief    This source file declaration the GameOver states and all the function definitions to render all the
text in the Game Over screen
________________________________________________________________________________________________________*/

typedef struct Wizard {
	CP_Color color;
	float xpos;
	float ypos;
	float size;
	char* words;
}Wizard;
/*----------------------------------------------------------------------------
@brief Get's the Wizard's x-pos and y-pos
------------------------------------------------------------------------------*/
CP_Vector GetWizardPosition();

/*----------------------------------------------------------------------------
@brief Initializes the Wizard's text body part
------------------------------------------------------------------------------*/
void InitWizard(void);

/*----------------------------------------------------------------------------
@brief Updates all the Wizard's body parts
------------------------------------------------------------------------------*/
void UpdateWizard(void);

/*----------------------------------------------------------------------------
@brief Draws the Wizard's body part
------------------------------------------------------------------------------*/
void DrawWizard(Wizard wiz_body_part);

/*----------------------------------------------------------------------------
@brief Initializes the images for the rotation icon and the mouse right click icon
------------------------------------------------------------------------------*/
void InitMouseIcon(void);

/*----------------------------------------------------------------------------
@brief Renders the mouse icon and the rotation icon
------------------------------------------------------------------------------*/
void RenderMouseIcon(void);

/*----------------------------------------------------------------------------
@brief Frees the images of the rotation and right click icon that was loaded previously
------------------------------------------------------------------------------*/
void FreeMouseIcons(void);