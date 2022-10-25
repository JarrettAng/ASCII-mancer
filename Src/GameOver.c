#include "cprocessing.h"
#include "ColorTable.h"
#include "GameOver.h"
#include "Grid.h"

// ---------------------------------
// Text and Button function definitions
// ---------------------------------

Text CreateText(char *words, float size) {
	Text New_Text;
	New_Text.font_size = size;
	New_Text.color;
	New_Text.words = words;
	return New_Text;
}

Button CreateButtonWithText(Text text, float width, float height, float pos_x, float pos_y) {
	Button New_Button;
	New_Button.width = width;
	New_Button.height = height;
	New_Button.pos_x = pos_x;
	New_Button.pos_y = pos_y;
	New_Button.color;
	New_Button.text = text;
	return New_Button;
}

int IsButtonPressed(Button Button_Check) {
	int answer = 0;
	float click_x = CP_Input_GetMouseX();
	float click_y = CP_Input_GetMouseY();
	if (click_x > Button_Check.pos_x - (Button_Check.width / 2.0) && click_y > Button_Check.pos_y - (Button_Check.height / 2.0) && click_x < Button_Check.pos_x + (Button_Check.width / 2.0) && click_y < Button_Check.pos_y + (Button_Check.height / 2.0)) {
		if (CP_Input_KeyReleased(MOUSE_BUTTON_1) == 1) {
			answer = 1;
		}
		else if (CP_Input_KeyReleased(MOUSE_BUTTON_1) == 0) {
			answer = 0;
		}
	}
	return answer;
}

void RenderText(Text Rendered_Text, float pos_x, float pos_y, CP_Color color) {
	Rendered_Text.color = color;
	CP_Settings_Fill(Rendered_Text.color); // Color of text
	CP_Settings_TextSize(Rendered_Text.font_size); // Size of text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE); // Origin of text is it's absolute center
	CP_Font_DrawText(Rendered_Text.words, pos_x, pos_y);
}

void RenderButtonWithText(Button Rendered_Button, CP_Color color, CP_Color stroke_color, float stroke_weight, CP_Color text_color) {
	Rendered_Button.color = color;
	// Render button underneath first
	CP_Settings_Fill(Rendered_Button.color); // Color of button
	CP_Settings_Stroke(stroke_color);
	CP_Settings_StrokeWeight(stroke_weight);
	CP_Settings_RectMode(CP_POSITION_CENTER); // Origin of button is at center
	CP_Graphics_DrawRect(Rendered_Button.pos_x, Rendered_Button.pos_y, Rendered_Button.width, Rendered_Button.height);
	// Render text on top of button
	RenderText(Rendered_Button.text, Rendered_Button.pos_x, Rendered_Button.pos_y, text_color);
}

//----------------------------------
// GameOver State definitions
//----------------------------------

CP_Font main_font;

void GameOverInit(void) {
	CP_System_Fullscreen();

	CP_Graphics_ClearBackground(CP_Color_Create(GAMEOVER_DARKGRAY));

	main_font = CP_Font_Load("Assets/PressStart2P-Regular.ttf");
}

void GameOverUpdate(void) {
	CP_Graphics_ClearBackground(CP_Color_Create(GAMEOVER_DARKGRAY));

	CP_Font_Set(main_font); // Uses main_font for all fonts

	// Game Over Title card
	Text Game_Over_Title = CreateText("Game Over !", HeaderFontSize);
	RenderText(Game_Over_Title, CP_System_GetDisplayWidth() / 2.f, CP_System_GetDisplayHeight() / 2.f - 300.f, CP_Color_Create(WHITE));

	// Stats
	int Moves = 10; // Trackable moves
	Text Moves_Made = CreateText("Moves made : 10", DefaultFontSize);
	RenderText(Moves_Made, CP_System_GetDisplayWidth() / 2.f, CP_System_GetDisplayHeight() / 2.f - 48.f, CP_Color_Create(WHITE));
	int Waves = 7; // Trackable waves
	Text Waves_Survived = CreateText("Waves survived : 7", DefaultFontSize);
	RenderText(Waves_Survived, CP_System_GetDisplayWidth() / 2.f, CP_System_GetDisplayHeight() / 2.f + 66.f, CP_Color_Create(WHITE));


	// Back button
	Text Grid = CreateText("Back", DefaultFontSize);
	Button Back_To_Grid = CreateButtonWithText(Grid, 400.f, 148.f, CP_System_GetDisplayWidth() / 2.f, CP_System_GetDisplayHeight() / 2.f + 300.f);
	RenderButtonWithText(Back_To_Grid, CP_Color_Create(GAMEOVER_RED), CP_Color_Create(WHITE), 20.f, CP_Color_Create(WHITE));
	if (IsButtonPressed(Back_To_Grid) == 1) CP_Engine_SetNextGameState(test_init, test_update, test_exit);


	if (CP_Input_KeyTriggered(KEY_Z) == 1) {
		CP_Engine_Terminate();
	}
}

void GameOverExit(void) {

}
