#include "Settings.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "MainMenu.h"
#include "Utils.h"


Slider volumeSlider;
SliderKnob volumeKnob;

Text settingsTxt;
Text masterVolumeTxt;
Text sfxVolumeTxt;
Text windowSizeTxt;

Button backBtn;

#pragma region FORWARD_DECLARATIONS

void IntializeTexts(void);
void IntializeButtons(void);
void DrawSlider(void);
void DrawSliderBob(void);

void LoadMainMenu(void);

#pragma endregion


void SettingsInit(void){

	Slider sliderData = {
	.start = CP_Vector_Set(500, 500),
	.end = CP_Vector_Set(1000, 500),

	.stroke = MENU_RED,
	.strokeWeight = 10
	};

	SliderKnob knobData = {
	.x = sliderData.start.x,
	.y = sliderData.start.y,
	.bobRadius = 25,

	.colour = WHITE,
	.stroke = MENU_RED,
	.strokeWeight = 10
	};

	volumeKnob = knobData;
	volumeSlider = sliderData;

	IntializeTexts();
	IntializeButtons();
}

void SettingsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);

	// Settings header
	RenderTexts();
	DrawButtons();
	HandleButtonClick();

	DrawSlider();
	DrawSliderBob();
}

void SettingsExit(void){
	FreeUI();
}

void DrawSlider(){
	CP_Settings_Stroke(volumeSlider.stroke);
	CP_Settings_StrokeWeight(volumeSlider.strokeWeight);

	CP_Graphics_DrawLine(volumeSlider.start.x, volumeSlider.start.y, volumeSlider.end.x, volumeSlider.end.y);
}

void DrawSliderBob(){
	CP_Settings_Fill(volumeKnob.colour);
	CP_Settings_Stroke(volumeKnob.stroke);
	CP_Settings_StrokeWeight(volumeKnob.strokeWeight);

	CP_Graphics_DrawCircle(CP_Math_ClampFloat(volumeKnob.x, volumeSlider.start.x, volumeSlider.end.x), CP_Math_ClampFloat(volumeKnob.y, volumeSlider.start.y, volumeSlider.end.y), volumeKnob.bobRadius * 2);
}

void IntializeTexts(){
	/*=======================Text Settings===================*/
	// Currently all texts in settings are using header/subHeader setting
	TextData baseTextData = {
		.color = MENU_WHITE,
		.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
		.hAlign = CP_TEXT_ALIGN_H_CENTER,
		.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
	};

	TextData headerData = baseTextData;
	headerData.textSize = 80 * GetHeightScale();

	TextData subHeaderData = baseTextData;
	subHeaderData.textSize = 50 * GetHeightScale();

	/*========================Settings Header============================*/
	Rect settingsTxtRect = {
		.x = GetWindowWidth() / 2,
		.y = GetWindowHeight() / 9,
	};

	headerData.text = "SETTINGS";
	IntializeText(&settingsTxt, settingsTxtRect, headerData);

	/*========================Volume Sub Headers============================*/
	Rect masterTxtRect = {
		.x = GetWindowWidth() / 5,
		.y = GetWindowHeight() / 3,
	};

	subHeaderData.text = "MASTER";
	IntializeText(&masterVolumeTxt, masterTxtRect, subHeaderData);

	Rect sfxTxtRect = {
		.x = GetWindowWidth() / 5,
		.y = GetWindowHeight() - GetWindowHeight() / 3,
	};

	subHeaderData.text = "SFX";
	IntializeText(&sfxVolumeTxt, sfxTxtRect, subHeaderData);

	/*========================Window Size Headers============================*/
	Rect windowSizeTxtRect = {
	.x = GetWindowWidth() - GetWindowWidth() / 5,
	.y = GetWindowHeight() / 3,
	};

	subHeaderData.text = "WINDOW SIZE";
	IntializeText(&windowSizeTxt, windowSizeTxtRect, subHeaderData);
}

void IntializeButtons(){
	GraphicData graphicData = {
		.color = TRANSPERANT,
		.strokeColor = TRANSPERANT,
		.strokeWeight = 0,
		// Draw from top left corner so menu button aligns.
		.imagePosMode = CP_POSITION_CENTER
	};

	TextData textData = {
		.color = MENU_WHITE,
		.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
		.textSize = 70 * GetHeightScale(),
		// Text will be drawn on the X Y pos of btn rect (which is top left corner),
		// So we shift text to top left.
		.hAlign = CP_TEXT_ALIGN_H_CENTER,
		.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
		.text = "BACK"
	};

	/*========================Start Button============================*/
	Rect backBtnRect = {
		.x = GetWindowWidth() / 2,
		.y = GetWindowHeight() - GetWindowHeight() / 8,
		.heigth = textData.textSize,
		.width = 400 * GetWidthScale(),
	};
	InitializeButton(&backBtn, backBtnRect, graphicData, textData, LoadMainMenu);
}

void LoadMainMenu(){
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}
