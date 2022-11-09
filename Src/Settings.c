#include <stdio.h>
#include <stdlib.h>
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

// Buffer to store string when converting int to char.
char* masterVolumeBuffer;
char* sfxVolumeBuffer;

#pragma region FORWARD_DECLARATIONS

void InitializeTexts(void);
void InitializeSliders(void);
static void InitializeButtons(void);
void UpdateMasterVolumeText(void);
void UpdateSFXVolumeText(void);
void InitializeVolumeBuffers(void);
void InitializeVolume(void);
void HandleVolumeControl(void);
void UpdateVolumes(void);
void UpdateKnobs(void);

void LoadMainMenu(void);


#pragma endregion

void SettingsInit(void){
	InitializeTexts();
	InitializeSliders();
	InitializeButtons();
	InitializeVolume();
	InitializeVolumeBuffers();

}

void SettingsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);

	// Settings header
	RenderTexts();
	RenderSliders();
	DrawButtons();
	HandleButtonClick();

	HandleVolumeControl();
}

void SettingsExit(void){
	ClearInteractCache();
	FreeUI();
	free(masterVolumeBuffer);
	free(sfxVolumeBuffer);
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
	// Offset between each slider.
	float sliderOffset = GetWindowHeight() / 10 * GetHeightScale();
	CP_Image sliderImg = CP_Image_Load("Assets/Slider.png");
	CP_Image knobImg = CP_Image_Load("Assets/Knob.png");
	// Offset of knob from ends of slider.
	float knobOffset = (float)CP_Image_GetWidth(sliderImg) / 12;

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
		.start = CP_Vector_Set(masterVolumeRect.x - CP_Image_GetWidth(sliderImg) / 2 + knobOffset, masterVolumeRect.y),
		.end = CP_Vector_Set(masterVolumeRect.x + CP_Image_GetWidth(sliderImg) / 2 - knobOffset, masterVolumeRect.y),
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
		.width = (float)CP_Image_GetWidth(sliderImg),
		.heigth = (float)CP_Image_GetHeight(sliderImg),
	};

	Line sfxVolumeLine = {
		.start = CP_Vector_Set(sfxVolumeRect.x - CP_Image_GetWidth(sliderImg) / 2 + knobOffset, sfxVolumeRect.y),
		.end = CP_Vector_Set(sfxVolumeRect.x + CP_Image_GetWidth(sliderImg) / 2 - knobOffset, sfxVolumeRect.y),
	};
	knobData.transform.x = sfxVolumeRect.x;
	knobData.transform.y = sfxVolumeRect.y;
	sfxVolumeSlider.lerpFactor = CP_Sound_GetGroupVolume(CP_SOUND_GROUP_SFX);
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

void InitializeVolume(){
	// Update knob position based on initial volume group lerp factor.
	UpdateKnobs();
	// Update actual volume based on initial volume group lerp factor.
	UpdateVolumes();
	// Volume buffer to store volume texts.
	InitializeVolumeBuffers();
	// Update volume text based on integer values.
	UpdateMasterVolumeText();
	UpdateSFXVolumeText();
}

void InitializeVolumeBuffers(){
	// -100- : 5 characters
	// +1 for null character.
	masterVolumeBuffer = malloc(sizeof(char) * 5 + 1);
	sfxVolumeBuffer = malloc(sizeof(char) * 5 + 1);
}

void HandleVolumeControl(){
	if (GetSliderHeld() != NULL){
		// Clamp knob between the limits of the slider.
		float xPos = CP_Math_ClampFloat(CP_Input_GetMouseX(), GetSliderHeld()->line.start.x, GetSliderHeld()->line.end.x);
		GetSliderHeld()->lerpFactor = (xPos - GetSliderHeld()->line.start.x) / (GetSliderHeld()->line.end.x - GetSliderHeld()->line.start.x);

		// Update position of knob on slider based on lerp factor.
		UpdateKnobs();
		// Update actual volumes.
		UpdateVolumes();
		UpdateMasterVolumeText();
		UpdateSFXVolumeText();
	}
}

void UpdateKnobs(){
	// Get knob x position based on lerp factor.
	masterVolumeSlider.knob.transform.x = masterVolumeSlider.lerpFactor * (masterVolumeSlider.line.end.x - masterVolumeSlider.line.start.x) + masterVolumeSlider.line.start.x;
	sfxVolumeSlider.knob.transform.x = sfxVolumeSlider.lerpFactor * (sfxVolumeSlider.line.end.x - sfxVolumeSlider.line.start.x) + sfxVolumeSlider.line.start.x;
}

void UpdateVolumes(){
	masterVolume = CP_Math_LerpInt(0, 100, masterVolumeSlider.lerpFactor);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, masterVolumeSlider.lerpFactor);

	sfxVolume = CP_Math_LerpInt(0, 100, sfxVolumeSlider.lerpFactor);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, sfxVolumeSlider.lerpFactor);
}

void UpdateMasterVolumeText(){
	// Convert int to string and store it in buffer.
	sprintf_s(masterVolumeBuffer, sizeof(masterVolumeBuffer), "-%d-", masterVolume);
	masterVolumeTxt.textData.text = masterVolumeBuffer;
}

void UpdateSFXVolumeText(){
	// Convert int to string and store it in buffer.
	sprintf_s(sfxVolumeBuffer, sizeof(sfxVolumeBuffer), "-%d-", sfxVolume);
	sfxVolumeTxt.textData.text = sfxVolumeBuffer;
}

void LoadMainMenu(){
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}
