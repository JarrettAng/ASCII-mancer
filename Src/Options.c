/*!
@file	  Options.c
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This source file for displaying the options screen.
________________________________________________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>
#include "Options.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "MainMenu.h"
#include "Utils.h"

Text settingsTxt;
Button backBtn;

#pragma region VOLUMES
Text masterTxt;
Text masterVolumeTxt;

Text sfxTxt;
Text sfxVolumeTxt;

Slider masterVolumeSlider;
Slider sfxVolumeSlider;

int masterVolume = 100;
int sfxVolume = 100;

// Buffer to store string when converting int to char.
// -100- : 5 characters
// +1 for null character.
char* masterVolumeBuffer[6];
char* sfxVolumeBuffer[6];
#pragma endregion

#pragma region WINDOW_SIZE
Text windowSizeTxt;

Button upBtn;
Button downBtn;
Text sizesTxt;
char* windowSizes[] = {
	"FULL SCREEN",		// 0
	"BORDERLESS WINDOW",// 1
	"1920 x 1080",		// 2
	"1280 x 720",		// 3
	"960 x 540"			// 4
};
static int currentWindowSize = 0;
#pragma endregion

#pragma region FORWARD_DECLARATIONS

void InitSettingsTexts(void);
void InitSettingsSliders(void);
void InitSettingsButtons(void);
void InitVolume(void);

void HandleVolumeControl(void);

void UpdateVolumes(void);
void UpdateKnobs(void);
void UpdateMasterVolumeText(void);
void UpdateSFXVolumeText(void);

void HandleWindowSizeHover(void);
void HandleWindowSizeScroll(int dir);

void UpdateWindowSize(void);
void WindowSizeScrollUp(void);
void WindowSizeScrollDown(void);

void LoadMainMenu(void);

#pragma endregion

void OptionsInit(void){
	InitSettingsTexts();
	InitSettingsSliders();
	InitSettingsButtons();
	InitVolume();
}

void OptionsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);

	// Render UI elements and handling UI interactions.
	UIManagerUpdate();

	// Handle adjusting of volume.
	HandleVolumeControl();

	// Handle back button click.
	HandleButtonClick();

	HandleWindowSizeHover();
}

void OptionsExit(void){
	FreeUIManager();
}

void InitSettingsTexts(){
	// Offfset between the volume texts.
	float volumeOffset = GetWindowHeight() / 4;
	// Offfset between the volume text and volume value text.
	float volumeValueOffset = GetWindowHeight() / 25;

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

	// Window sizes text.
	TextData windowSizeData = baseTextData;
	windowSizeData.textSize = 40 * GetHeightScale();

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

	/*====================Window Sizes Text==========================*/
	Rect windowSizesTxtRect = {
	.x = GetWindowWidth() - GetWindowWidth() / 4,
	.y = (float)(windowSizeTxtRect.y + GetWindowHeight() / 4.5),
	};
	windowSizeData.text = windowSizes[currentWindowSize];
	InitializeText(&sizesTxt, windowSizesTxtRect, windowSizeData);
	/*==============================================================*/
}

void InitSettingsSliders(){
	// Offset between each slider.
	float sliderOffset = GetWindowHeight() / 10;

	// Load and cache images of slider and knob.
	CP_Image sliderImg = CP_Image_Load("Assets/Slider.png");
	CP_Image knobImg = CP_Image_Load("Assets/Knob.png");

	// Offset of knob from ends of slider.
	float knobOffset = (float)CP_Image_GetWidth(sliderImg) / 12 * GetWidthScale();

	SliderKnob knobData = {
		.img = knobImg,
		.radius = (float)CP_Image_GetWidth(knobImg) / 2 * GetWidthScale(),
		.transform.width = (float)CP_Image_GetWidth(knobImg) * GetWidthScale(),
		.transform.heigth = (float)CP_Image_GetHeight(knobImg) * GetHeightScale(),
	};

	/*=================Master Volume Slider=========================*/
	Rect masterVolumeRect = {
		.x = masterTxt.transform.x,
		.y = masterTxt.transform.y + sliderOffset,
		.width = (float)CP_Image_GetWidth(sliderImg) * GetWidthScale(),
		.heigth = (float)CP_Image_GetHeight(sliderImg) * GetHeightScale(),
	};

	Line masterVolumeLine = {
		// Vector position of the start and end of slider.
		.start = CP_Vector_Set(masterVolumeRect.x - CP_Image_GetWidth(sliderImg) / 2 * GetWidthScale() + knobOffset , masterVolumeRect.y),
		.end = CP_Vector_Set(masterVolumeRect.x + CP_Image_GetWidth(sliderImg) / 2 * GetWidthScale() - knobOffset , masterVolumeRect.y),
	};
	knobData.transform.x = masterVolumeRect.x;
	knobData.transform.y = masterVolumeRect.y;
	masterVolumeSlider.lerpFactor = CP_Sound_GetGroupVolume(CP_SOUND_GROUP_MUSIC);
	InitializeSlider(&masterVolumeSlider, masterVolumeRect, masterVolumeLine, sliderImg, knobData);
	/*==============================================================*/

	/*=================SFX Volume Slider============================*/
	Rect sfxVolumeRect = {
		.x = sfxTxt.transform.x,
		.y = sfxTxt.transform.y + sliderOffset,
		.width = (float)CP_Image_GetWidth(sliderImg) * GetWidthScale(),
		.heigth = (float)CP_Image_GetHeight(sliderImg) * GetHeightScale(),
	};

	Line sfxVolumeLine = {
		// Vector position of the start and end of slider.
		.start = CP_Vector_Set(sfxVolumeRect.x - CP_Image_GetWidth(sliderImg) / 2 * GetWidthScale() + knobOffset, sfxVolumeRect.y),
		.end = CP_Vector_Set(sfxVolumeRect.x + CP_Image_GetWidth(sliderImg) / 2 * GetWidthScale() - knobOffset, sfxVolumeRect.y),
	};
	knobData.transform.x = sfxVolumeRect.x;
	knobData.transform.y = sfxVolumeRect.y;
	sfxVolumeSlider.lerpFactor = CP_Sound_GetGroupVolume(CP_SOUND_GROUP_SFX);
	InitializeSlider(&sfxVolumeSlider, sfxVolumeRect, sfxVolumeLine, sliderImg, knobData);
	/*==============================================================*/
}

void InitSettingsButtons(){
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

	/*=================Window Size Button===========================*/
	CP_Image windowSizeUpBtn = CP_Image_Load("Assets/WindowSizeUpBtn.png");
	CP_Image windowSizeDownBtn = CP_Image_Load("Assets/WindowSizeDownBtn.png");
	float windowSizeBtnOffet = (float)GetWindowHeight() / 4;

	GraphicData windowSizeBtnGraphicData = {
		.imagePosMode = CP_POSITION_CENTER,
		.imageFilterMode = CP_IMAGE_FILTER_NEAREST
	};
	Rect windowSizeRect = {
		.x = GetWindowWidth() - GetWindowWidth() / 4,
		.y = windowSizeTxt.transform.y + GetWindowHeight() / 10,
		.heigth = (float)CP_Image_GetHeight(windowSizeUpBtn) * GetHeightScale(),
		.width = (float)CP_Image_GetWidth(windowSizeUpBtn) * GetWidthScale(),
	};

	TextData windowSizeBtnTextData = { .text = NULL };
	windowSizeBtnGraphicData.img = windowSizeUpBtn;
	InitializeButton(&upBtn, windowSizeRect, windowSizeBtnGraphicData, windowSizeBtnTextData, WindowSizeScrollUp);

	windowSizeBtnGraphicData.img = windowSizeDownBtn;
	windowSizeRect.y += windowSizeBtnOffet;
	InitializeButton(&downBtn, windowSizeRect, windowSizeBtnGraphicData, windowSizeBtnTextData, WindowSizeScrollDown);
	/*==============================================================*/
}

void InitVolume(){
	// Update knob position based on initial volume group lerp factor.
	UpdateKnobs();
	// Update actual volume based on initial volume group lerp factor.
	UpdateVolumes();
	// Update volume text based on integer values.
	UpdateMasterVolumeText();
	UpdateSFXVolumeText();
}

void HandleVolumeControl(){
	// Reset previous slider knob to original size.
	if (GetSliderHeld() == NULL){
		if (GetPrevSliderHeld() != NULL){
			GetPrevSliderHeld()->knob.transform.width = GetPrevSliderHeld()->knob.transform.cachedSize.x;
			GetPrevSliderHeld()->knob.transform.heigth = GetPrevSliderHeld()->knob.transform.cachedSize.y;
		}
		return;
	}
	// Clamp knob between the limits of the slider.
	float xPos = CP_Math_ClampFloat(CP_Input_GetMouseX(), GetSliderHeld()->line.start.x, GetSliderHeld()->line.end.x);
	GetSliderHeld()->lerpFactor = (xPos - GetSliderHeld()->line.start.x) / (GetSliderHeld()->line.end.x - GetSliderHeld()->line.start.x);

	// Enlarge current slider knob held.
	GetSliderHeld()->knob.transform.width = GetSliderHeld()->knob.transform.cachedSize.x * 1.2;
	GetSliderHeld()->knob.transform.heigth = GetSliderHeld()->knob.transform.cachedSize.y * 1.2;

	// Update position of knob on slider based on lerp factor.
	UpdateKnobs();
	// Update actual volumes.
	UpdateVolumes();
	// Update text of volumes.
	UpdateMasterVolumeText();
	UpdateSFXVolumeText();
}

void UpdateKnobs(){
	// Get master knob x position based on lerp factor.
	masterVolumeSlider.knob.transform.x = masterVolumeSlider.lerpFactor * (masterVolumeSlider.line.end.x - masterVolumeSlider.line.start.x) + masterVolumeSlider.line.start.x;
	// Get SFX knob x position based on lerp factor.
	sfxVolumeSlider.knob.transform.x = sfxVolumeSlider.lerpFactor * (sfxVolumeSlider.line.end.x - sfxVolumeSlider.line.start.x) + sfxVolumeSlider.line.start.x;
}

void HandleWindowSizeHover(){
	// Reset previous button to original size.
	if (GetBtnHovered() == NULL){
		if (GetPrevBtnHovered() != NULL){
			GetPrevBtnHovered()->transform.width = GetPrevBtnHovered()->transform.cachedSize.x;
			GetPrevBtnHovered()->transform.heigth = GetPrevBtnHovered()->transform.cachedSize.y;
		}
		return;
	}
	// Enlarge current hovered button.
	GetBtnHovered()->transform.width = GetBtnHovered()->transform.cachedSize.x * 1.3;
	GetBtnHovered()->transform.heigth = GetBtnHovered()->transform.cachedSize.y * 1.3;
}

void WindowSizeScrollDown(){
	HandleWindowSizeScroll(0);
}

void WindowSizeScrollUp(){
	HandleWindowSizeScroll(1);
}

void HandleWindowSizeScroll(int dir){
	// Scroll up/down based on button press.
	(dir == 0) ? currentWindowSize++ : currentWindowSize--;

	// Update current window size variable.
	currentWindowSize = (currentWindowSize < 0) ? sizeof(windowSizes) / sizeof(windowSizes[0]) - 1 : currentWindowSize;
	currentWindowSize = (currentWindowSize > (sizeof(windowSizes) / sizeof(windowSizes[0]) - 1)) ? 0 : currentWindowSize;
	sizesTxt.textData.text = windowSizes[currentWindowSize];
	// Update window size based on current window size variable.
	UpdateWindowSize();
}

void UpdateWindowSize(){
	// Update window size based on current window size variable.
	switch (currentWindowSize)
	{
	case 0:
		CP_System_Fullscreen();
		break;
	case 1:
		CP_System_SetWindowSize(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
		break;
	case 2:
		CP_System_SetWindowSize(1920, 1080);
		break;
	case 3:
		CP_System_SetWindowSize(1280, 720);
		break;
	case 4:
		CP_System_SetWindowSize(960, 540);
		break;
	default:
		break;
	}
	// Update the scaling of all UI element.
	UpdateUIScale();
	// Refresh scene.
	CP_Engine_SetNextGameStateForced(OptionsInit, OptionsUpdate, OptionsExit);
}

void UpdateVolumes(){
	// Update master volume.
	masterVolume = CP_Math_LerpInt(0, 100, masterVolumeSlider.lerpFactor);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, masterVolumeSlider.lerpFactor);

	// Update SFX volume.
	sfxVolume = CP_Math_LerpInt(0, 100, sfxVolumeSlider.lerpFactor);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, sfxVolumeSlider.lerpFactor);
}

void UpdateMasterVolumeText(){
	// Convert int to string and store it in buffer.
	sprintf_s((char* const)masterVolumeBuffer, sizeof(masterVolumeBuffer), "-%d-", masterVolume);
	masterVolumeTxt.textData.text = (char*)masterVolumeBuffer;
}

void UpdateSFXVolumeText(){
	// Convert int to string and store it in buffer.
	sprintf_s((char* const)sfxVolumeBuffer, sizeof(sfxVolumeBuffer), "-%d-", sfxVolume);
	sfxVolumeTxt.textData.text = (char*)sfxVolumeBuffer;
}

void LoadMainMenu(){
	// Transition back to main menu.
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}
