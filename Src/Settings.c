#include "Settings.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "MainMenu.h"
#include "Utils.h"

Text settingsTxt;

Text masterTxt;
Text masterVolumeTxt;

Text sfxTxt;
Text sfxVolumeTxt;

Text windowSizeTxt;

Button backBtn;

Slider masterVolumeSlider;
Slider sfxVolumeSlider;

int masterVolume = 100;
int sfxVolume = 100;

#pragma region FORWARD_DECLARATIONS

void InitializeTexts(void);
void InitializeSliders(void);
static void InitializeButtons(void);
void LoadMainMenu(void);

#pragma endregion


void SettingsInit(void){
	InitializeTexts();
	InitializeSliders();
	InitializeButtons();
}

void SettingsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);

	// Settings header
	RenderTexts();
	RenderSliders();
	DrawButtons();
	HandleButtonClick();
}

void SettingsExit(void){
	ClearInteractCache();
	FreeUI();
}

void InitializeTexts(){
	// Offfset between the volume texts.
	float volumeOffset = GetWindowHeight() / 4 * GetHeightScale();
	// Offfset between the volume text and volume value text.
	float volumeValueOffset = GetWindowHeight() / 25 * GetHeightScale();

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

	// Sub header includes master, sfx and window size.
	TextData subHeaderData = baseTextData;
	subHeaderData.textSize = 60 * GetHeightScale();

	// Master and sfx volume amount.
	TextData volumeValueData = baseTextData;
	volumeValueData.textSize = 30 * GetHeightScale();

	/*======================Settings Text==========================*/
	Rect settingsTxtRect = {
		.x = GetWindowWidth() / 2,
		.y = GetWindowHeight() / 9,
	};
	headerData.text = "SETTINGS";
	InitializeText(&settingsTxt, settingsTxtRect, headerData);
	/*=============================================================*/

	/*========================Master Text==========================*/
	Rect masterTxtRect = {
		.x = GetWindowWidth() / 4,
		.y = GetWindowHeight() / 3,
	};
	subHeaderData.text = "MASTER";
	InitializeText(&masterTxt, masterTxtRect, subHeaderData);
	/*=============================================================*/

	/*====================Master Volume Text=======================*/
	Rect masterVolumeTextRect = {
		.x = masterTxtRect.x,
		.y = masterTxtRect.y + volumeValueOffset
	};
	volumeValueData.text = "-100-";
	InitializeText(&masterVolumeTxt, masterVolumeTextRect, volumeValueData);
	/*=============================================================*/

	/*========================SFX Text=============================*/
	Rect sfxTxtRect = {
		.x = GetWindowWidth() / 4,
		.y = masterTxtRect.y + volumeOffset,
	};
	subHeaderData.text = "SFX";
	InitializeText(&sfxTxt, sfxTxtRect, subHeaderData);
	/*==============================================================*/

	/*=======================SFX Volume Text========================*/
	Rect sfxVolumeTextRect = {
		.x = sfxTxtRect.x,
		.y = sfxTxtRect.y + volumeValueOffset
	};
	volumeValueData.text = "-100-";
	InitializeText(&sfxVolumeTxt, sfxVolumeTextRect, volumeValueData);
	/*==============================================================*/

	/*====================Window Size Text==========================*/
	Rect windowSizeTxtRect = {
	.x = GetWindowWidth() - GetWindowWidth() / 4,
	.y = GetWindowHeight() / 3,
	};
	subHeaderData.text = "WINDOW SIZE";
	InitializeText(&windowSizeTxt, windowSizeTxtRect, subHeaderData);
	/*==============================================================*/
}

void InitializeSliders(){
	float sliderOffset = GetWindowHeight() / 10 * GetHeightScale();
	CP_Image sliderImg = CP_Image_Load("Assets/Slider.png");
	CP_Image knobImg = CP_Image_Load("Assets/Knob.png");

	SliderKnob knobData = {
		.img = knobImg,
		.radius = (float)CP_Image_GetWidth(knobImg) / 2,
		.transform.width = (float)CP_Image_GetWidth(knobImg),
		.transform.heigth = (float)CP_Image_GetHeight(knobImg),
	};

	/*=================Master Volume Slider=========================*/
	Rect masterVolumeRect = {
		.x = masterTxt.transform.x,
		.y = masterTxt.transform.y + sliderOffset,
		.width = (float)CP_Image_GetWidth(sliderImg),
		.heigth = (float)CP_Image_GetHeight(sliderImg),
	};

	Line masterVolumeLine = {
		.start = CP_Vector_Set(masterVolumeRect.x - CP_Image_GetWidth(sliderImg) / 2, masterVolumeRect.y),
		.end = CP_Vector_Set(masterVolumeRect.x + CP_Image_GetWidth(sliderImg) / 2, masterVolumeRect.y),
	};
	InitializeSlider(&masterVolumeSlider, masterVolumeRect, masterVolumeLine, sliderImg, knobData);
	/*==============================================================*/

	/*=================SFX Volume Slider============================*/
	Rect sfxVolumeRect = {
		.x = sfxTxt.transform.x,
		.y = sfxTxt.transform.y + sliderOffset,
		.width = (float)CP_Image_GetWidth(sliderImg),
		.heigth = (float)CP_Image_GetHeight(sliderImg),
	};

	Line sfxVolumeLine = {
		.start = CP_Vector_Set(sfxVolumeRect.x - CP_Image_GetWidth(sliderImg) / 2, sfxVolumeRect.y),
		.end = CP_Vector_Set(sfxVolumeRect.x + CP_Image_GetWidth(sliderImg) / 2, sfxVolumeRect.y),
	};
	InitializeSlider(&sfxVolumeSlider, sfxVolumeRect, sfxVolumeLine, sliderImg, knobData);
	/*==============================================================*/
}

void InitializeButtons(){
	GraphicData graphicData = {
		.color = TRANSPERANT,
		.strokeColor = TRANSPERANT,
		.strokeWeight = 0,
		.imagePosMode = CP_POSITION_CENTER
	};

	TextData textData = {
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
		.heigth = textData.textSize,
		.width = 400 * GetWidthScale(),
	};
	InitializeButton(&backBtn, backBtnRect, graphicData, textData, LoadMainMenu);
	/*==============================================================*/
}

void LoadMainMenu(){
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}
