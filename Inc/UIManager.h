/*!
@file	  UIManager.h
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This header file contains function declartion that handles UI interactions.
________________________________________________________________________________________________________*/

#pragma once

#include "Utils.h"	// For callback function

#define MAX_UI_BUTTONS 20
#define MAX_UI_SLIDERS 5
#define MAX_UI_TEXT 20
#define MAX_UI_TEXTBOX 20

typedef struct Rect {
	float x;
	float y;
	CP_Vector cachedPos;
	float width;
	float heigth;
}Rect;

typedef struct Line {
	CP_Vector start;
	CP_Vector end;
}Line;

typedef struct GraphicData {
	CP_POSITION_MODE imagePosMode;
	CP_IMAGE_FILTER_MODE imageFilterMode;
	CP_Color color;
	CP_Color strokeColor;
	float strokeWeight;
	CP_Image img;
}GraphicData;

typedef struct TextData {
	CP_Font font;
	float textSize;
	CP_Color color;
	CP_TEXT_ALIGN_HORIZONTAL hAlign;
	CP_TEXT_ALIGN_VERTICAL vAlign;
	char* text;
}TextData;

typedef struct Button {
	Rect transform;
	GraphicData graphicData;
	TextData textData;
	Callback callBack;
}Button;

typedef struct Text {
	Rect transform;
	TextData textData;
}Text;

typedef struct SliderKnob{
	Rect transform;
	float radius;
	CP_Image img;
}SliderKnob;

typedef struct Slider{
	Rect transform;
	Line line;
	float lerpFactor;
	CP_Image img;
	SliderKnob knob;
}Slider;

/*
@brief InitializeButton will initialize the given button with the data and cache the button.
*/
void InitializeButton(Button* btn, Rect transform, GraphicData graphicsData, TextData textData, Callback callBack);

/*
@brief InitializeText will initialize the given text with the data and cache the text.
*/
void InitializeText(Text* txt, Rect transform, TextData data);

/*
@brief InitializeTextBox will initialize the given textbox with the data and cache the textbox.
*/
void InitializeTextBox(Text* txt, Rect transform, TextData data);

/*
@brief InitializeSlider will initialize the given text with the data and cache the slider.
*/
void InitializeSlider(Slider* slider, Rect transform, Line line, CP_Image img, SliderKnob knobData);

/*
@brief RenderButton will render the given button on screen if it is initialized.
*/
void RenderButton(Button* btn);

/*
@brief RenderButtons will render all button initialized.
*/
void RenderButtons(void);

/*
@brief RenderText will render the given text on screen if it is initialized.
*/
void RenderText(Text* txt);

/*
@brief RenderTexts will render all texts initialized.
*/
void RenderTexts(void);

/*
@brief RenderTextBoxes will render all text boxes initialized.
*/
void RenderTextBoxes(void);

/*
@brief RenderSlider will render the given slider on screen if it is initialized.
*/
void RenderSlider(Slider* slider);

/*
@brief RenderSliders will render all sliders initialized.
*/
void RenderSliders(Void);

/*
@brief Update cprocessing graphics settings based on the given data.
*/
void SetGraphicSetting(GraphicData data);

/*
@brief Update cprocessing text settings based on the given data.
*/
void SetTextSetting(TextData data);

/*
@brief Invoke callback function of button if clicked.
*/
void HandleButtonClick(void);

/*
@brief Get the button that was clicked in this frame.
*/
Button* GetButtonClick(void);


/*
@brief Get the last button clicked.
*/
Button* GetBtnClicked(void);

/*
@brief Get the button that was hovered in this frame.
*/
Button* GetButtonHover(void);

/*
@brief Get the cached button hovered in the frame. To avoid extra checks.
*/
Button* GetBtnHovered(void);

/*
@brief Get the last button hovered.
*/
Button* GetPrevBtnHovered(void);

/*
@brief Get the slider that is held.
*/
Slider* GetSliderHeld(void);

/*
@brief Clear all cached interactions.
*/
void ClearInteractCache(void);

/*
@brief Free all UI elements.
*/
void FreeUI();
void FreeText(void);
void FreeButton(void);
void FreeSlider(void);
