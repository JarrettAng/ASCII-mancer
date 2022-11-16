#include "Credits.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "MainMenu.h"

Text creditsTxt;
Text testTxt;
Text testTxt2;
Button backBtn;

#pragma region FORWARD_DECLARATIONS
void InitCreditsTexts(void);
void InitCreditsButtons(void);
static void LoadMainMenu(void);
#pragma endregion


void CreditsInit(void){
	InitCreditsTexts();
	InitCreditsButtons();
}

void CreditsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);
	RenderTexts();
	CP_Settings_Fill(MENU_BLACK);
	CP_Graphics_DrawRect(GetWindowWidth() / 2, 0, GetWindowWidth(), GetWindowHeight() / 6);
	CP_Settings_Fill(MENU_BLACK);
	CP_Graphics_DrawRect(GetWindowWidth() / 2, GetWindowHeight() - GetWindowHeight() / 8, GetWindowWidth(), GetWindowHeight() / 4);
	RenderButtons();

	HandleButtonClick();
	testTxt.transform.y -= CP_System_GetDt() * 300;
	testTxt2.transform.y -= CP_System_GetDt() * 300;
}

void CreditsExit(void){
	ClearInteractCache();
	FreeUI();
}

void InitCreditsTexts(){
	/*==============================Text Settings========================*/
	TextData baseTextData = {
		.color = MENU_WHITE,
		.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
		.hAlign = CP_TEXT_ALIGN_H_CENTER,
		.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
	};
	// Settings text.
	TextData headerData = baseTextData;
	headerData.textSize = 80 * GetHeightScale();

	Rect testTxtRect = {
		.x = GetWindowWidth() / 2,
		.y = GetWindowHeight() / 9,
	};

	headerData.text = "LIGMA";
	testTxtRect.y = GetWindowHeight() + 100;
	InitializeText(&testTxt, testTxtRect, headerData);

	headerData.text = "BALLS";
	testTxtRect.y = GetWindowHeight() + 1000;
	InitializeText(&testTxt2, testTxtRect, headerData);

}

void InitCreditsButtons(){
	/*========================Back Button===========================*/
	GraphicData backBtnGraphicsData = {
		.color = TRANSPERANT,
		.strokeColor = TRANSPERANT,
		.strokeWeight = 0,
		.imagePosMode = CP_POSITION_CENTER
	};

	TextData backBtnTextData = {
		.color = MENU_WHITE,
		.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
		.textSize = 70 * GetHeightScale(),
		.hAlign = CP_TEXT_ALIGN_H_CENTER,
		.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
		.text = "BACK"
	};

	Rect backBtnRect = {
		.x = GetWindowWidth() / 2,
		.y = GetWindowHeight() - GetWindowHeight() / 8,
		.heigth = backBtnTextData.textSize,
		.width = 400 * GetWidthScale(),
	};
	InitializeButton(&backBtn, backBtnRect, backBtnGraphicsData, backBtnTextData, LoadMainMenu);
	/*==============================================================*/
}

void LoadMainMenu(){
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}

