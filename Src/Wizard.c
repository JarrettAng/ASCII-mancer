#include "cprocessing.h"
#include "ColorTable.h"
#include "Grid.h"
#include "Hearts.h"
#include "GameOver.h"
#include "Wizard.h"

Wizard wiz_body;
Wizard wiz_hat;
Wizard wiz_staff;
CP_Image rotation_icon;
CP_Image right_click_icon;

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

void DrawWizard(Wizard wiz_body_part) {
	CP_Settings_Fill(wiz_body_part.color);
	CP_Settings_TextSize(wiz_body_part.size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(wiz_body_part.words, wiz_body_part.xpos, wiz_body_part.ypos);
}

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

void InitMouseIcon(void) {
	rotation_icon = CP_Image_Load("Assets/RotationIcon.png");
	right_click_icon = CP_Image_Load("Assets/MouseRightClickIcon.png");
}
/*
float static time_elapsed = 0.f;
void RenderMouseIcon(void) {
	time_elapsed = CP_System_GetDt();
	float tick = time_elapsed / 
	CP_Image_DrawAdvanced(rotation_icon, CP_Input_GetMouseX(), CP_Input_GetMouseY(), 100.f, 100.f, 255, )
}
*/
void AnimationWizard() {

}

void HurtWizard() {
	
}

void DefeatWizard() {

}