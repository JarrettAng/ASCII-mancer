/*!
@file	  Wizard.c
@author	  Justine Ilao (justine.c@digipen.edu)
@date     26/11/2022
@brief    This source file defines the GameOver states and all the function definitions to render all the
		  text in the Game Over screen

@license  Copyright © 2022 DigiPen, All rights reserved.
________________________________________________________________________________________________________*/

#include "cprocessing.h"
#include "ColorTable.h"
#include "Grid.h"
#include "Hearts.h"
#include "GameOver.h"
#include "Wizard.h"
#include "TPlayerHeld.h"

Wizard wiz_body;
Wizard wiz_hat;
Wizard wiz_staff;
CP_Image rotation_icon;
CP_Image right_click_icon;

/*----------------------------------------------------------------------------
@brief Initializes the Wizard's text body part
------------------------------------------------------------------------------*/
void InitWizard(void) {
	// Body stats
	wiz_body.color = TETRIS_COLOR;
	wiz_body.xpos = CP_System_GetWindowWidth() / 15.f;
	wiz_body.ypos = CP_System_GetWindowHeight() / 2.2f;
	wiz_body.size = GetCellSize() / 1.5f;
	wiz_body.words = "@";

	// Hat stats
	wiz_hat.color = TETRIS_COLOR;
	wiz_hat.xpos = wiz_body.xpos;
	wiz_hat.ypos = wiz_body.ypos - GetCellSize() / 1.9f;
	wiz_hat.size = wiz_body.size;
	wiz_hat.words = "~";

	// Staff stats
	wiz_staff.color = TETRIS_COLOR;
	wiz_staff.xpos = wiz_body.xpos + GetCellSize() / 1.7f;
	wiz_staff.ypos = wiz_body.ypos;
	wiz_staff.size = wiz_body.size * 0.8f;
	wiz_staff.words = "/";
}

/*----------------------------------------------------------------------------
@brief Draws the Wizard's body part
------------------------------------------------------------------------------*/
void DrawWizard(Wizard wiz_body_part) {
	CP_Settings_Fill(wiz_body_part.color);
	CP_Settings_TextSize(wiz_body_part.size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(wiz_body_part.words, wiz_body_part.xpos, wiz_body_part.ypos);
}

/*----------------------------------------------------------------------------
@brief Updates all the Wizard's body parts
------------------------------------------------------------------------------*/
void UpdateWizard(void) {
	// Body update
	DrawWizard(wiz_body);

	// Hat update
	DrawWizard(wiz_hat);
	CP_Settings_TextSize(wiz_hat.size * 0.9f);
	CP_Font_DrawText("^", wiz_hat.xpos, wiz_hat.ypos - GetCellSize() / 20.f);

	// Staff update
	DrawWizard(wiz_staff);
}

/*----------------------------------------------------------------------------
@brief Get's the Wizard's x-pos and y-pos
------------------------------------------------------------------------------*/
CP_Vector GetWizardPosition(void){
	return CP_Vector_Set(wiz_body.xpos,wiz_body.ypos);
}

/*----------------------------------------------------------------------------
@brief Initializes the images for the rotation icon and the mouse right click icon
------------------------------------------------------------------------------*/
void InitMouseIcon(void) {
	rotation_icon = CP_Image_Load("Assets/RotationIcon.png");
	right_click_icon = CP_Image_Load("Assets/MouseRightClickIcon.png");
}

/*----------------------------------------------------------------------------
@brief Renders the mouse icon and the rotation icon
------------------------------------------------------------------------------*/
float static time_elapsed = 0.f;
void RenderMouseIcon(void) {

	float tick = time_elapsed / 3.f;
	if (tick >= 1.f) {
		time_elapsed = 0;
	}
	time_elapsed += CP_System_GetDt();
	float rotation = CP_Math_LerpFloat(0.f, 360.f, tick);
	if (IsPieceHeld() == TRUE) {
		CP_Image_DrawAdvanced(rotation_icon, CP_Input_GetMouseX() + GetCellSize() / 3.f, CP_Input_GetMouseY() + GetCellSize() / 3.f, GetCellSize() / 2.f, GetCellSize() / 2.f, 200, rotation);
		CP_Image_DrawAdvanced(right_click_icon, CP_Input_GetMouseX() + GetCellSize() / 1.5f, CP_Input_GetMouseY() + GetCellSize() / 1.5f, GetCellSize() / 3.f, GetCellSize() / 3.f, 200, 0.f);
	}
}

/*----------------------------------------------------------------------------
@brief Frees the images of the rotation and right click icon that was loaded previously
------------------------------------------------------------------------------*/
void FreeMouseIcons(void) {
	CP_Image_Free(&rotation_icon);
	CP_Image_Free(&right_click_icon);
}
