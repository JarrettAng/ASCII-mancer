#include "cprocessing.h"
#include "ColorTable.h"
#include "Grid.h"
#include "Hearts.h"

typedef struct Wizard {
	CP_Color color;
	float xpos;
	float ypos;
	float size;
	char* words;
}Wizard;

Wizard wiz_body;
Wizard wiz_hat;
Wizard wiz_staff;

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

void UpdateWizard(void) {
	// Body update
	CP_Settings_Fill(wiz_body.color);
	CP_Settings_TextSize(wiz_body.size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(wiz_body.words, wiz_body.xpos, wiz_body.ypos);

	// Hat update
	CP_Settings_Fill(wiz_hat.color);
	CP_Settings_TextSize(wiz_hat.size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(wiz_hat.words, wiz_hat.xpos, wiz_hat.ypos);
	CP_Settings_TextSize(wiz_hat.size * 0.9f);
	CP_Font_DrawText("^", wiz_hat.xpos, wiz_hat.ypos - GetCellSize() / 20.f);

	// Staff update
	CP_Settings_Fill(wiz_staff.color);
	CP_Settings_TextSize(wiz_staff.size);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(wiz_staff.words, wiz_staff.xpos, wiz_staff.ypos);
}

void AnimationWizard() {

}

void HurtWizard() {
	
}

void DefeatWizard() {

}