#pragma once

#include "Utils.h"	// For callback function

#define MAX_UI_BUTTONS 20

typedef struct GraphicData {
	CP_POSITION_MODE imagePosMode;
	CP_IMAGE_FILTER_MODE imageFilterMode;
	CP_Color color;
	CP_Color strokeColor;
	float strokeWeight;
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

/*Button*/
struct Button;

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

void InitializeButton(Button* btn, Rect transform, GraphicData graphicsData, TextData textData, Callback callBack);
void DrawButtons(void);
void SetGraphicSetting(GraphicData data);
void SetTextSetting(TextData data);
void HandleButtonClick(void);
Button* GetButtonClick(void);
Button* GetButtonHover(void);
Button* GetPrevBtnClicked(void);
Button* GetPrevBtnHovered(void);
void FreeButton(void);
