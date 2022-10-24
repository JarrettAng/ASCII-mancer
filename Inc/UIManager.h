#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#define MAX_UI_BUTTONS 20

typedef struct GraphicData {
	CP_POSITION_MODE imagePosMode;
	CP_IMAGE_FILTER_MODE imageFilterMode;
	CP_Color color;
	float strokeWeigth;
}GraphicData;

typedef struct TextData {
	float textSize;
	CP_Color color;
	CP_TEXT_ALIGN_HORIZONTAL hAlign;
	CP_TEXT_ALIGN_VERTICAL vAlign;
	char* text;
}TextData;

typedef struct Rect {
	float x;
	float y;
	float width;
	float heigth;
}Rect;

/*Button*/
struct Button;

typedef void(*Callback)();

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
void CheckForButtonClick(void);
_Bool IsAreaClicked(float areaX, float areaY, float areaWidth, float areaHeigth, float clickX, float clickY, CP_POSITION_MODE mode);

#endif