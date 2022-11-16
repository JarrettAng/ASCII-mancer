#include "Credits.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "MainMenu.h"

Text settingsTxt;
Button backBtn;

#pragma region FORWARD_DECLARATIONS
void InitCreditsButtons(void);
static void LoadMainMenu(void);
#pragma endregion


void CreditsInit(void){
	InitCreditsButtons();
}

void CreditsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);
	RenderButtons();
	HandleButtonClick();
}

void CreditsExit(void){
	ClearInteractCache();
	FreeUI();
}

void InitCreditsButtons(){
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

	/*========================Back Button===========================*/
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

