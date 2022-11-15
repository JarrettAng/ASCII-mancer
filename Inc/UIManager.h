#pragma once

#include "Utils.h"	// For callback function

#define MAX_UI_BUTTONS 20
#define MAX_UI_SLIDERS 5
#define MAX_UI_TEXT 20

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

typedef struct Rect {
	float x;
	float y;
	CP_Vector cachedPos;
	float width;
	float heigth;
}Rect;

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

typedef struct Line {
	CP_Vector start;
	CP_Vector end;
}Line;

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

void InitializeButton(Button* btn, Rect transform, GraphicData graphicsData, TextData textData, Callback callBack);
void InitializeText(Text* txt, Rect transform, TextData data);
void InitializeSlider(Slider* slider, Rect transform, Line line, CP_Image img, SliderKnob knobData);

void RenderButton(Button* btn);
void RenderButtons(void);

void RenderText(Text* txt);
void RenderTexts(void);

void RenderSlider(Slider* slider);
void RenderSliders(Void);

void SetGraphicSetting(GraphicData data);
void SetTextSetting(TextData data);

void HandleButtonClick(void);
Button* GetButtonClick(void);

Button* GetBtnClicked(void);

Button* GetButtonHover(void);
Button* GetPrevBtnHovered(void);

Button* GetBtnHovered(void);
Slider* GetSliderHeld(void);
void ClearInteractCache(void);

void FreeUI();
void FreeText(void);
void FreeButton(void);
void FreeSlider(void);
