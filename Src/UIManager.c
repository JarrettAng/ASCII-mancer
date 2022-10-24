#include <cprocessing.h>
#include "UIManager.h"

Button* btns[MAX_UI_BUTTONS];
int btnsCount = 0;

void InitializeButton(Button* btn, Rect transform, GraphicData graphicsData, TextData textData) {
	btn->transform = transform;
	btn->graphicData = graphicsData;
	btn->textData = textData;

	btns[btnsCount++] = btn;
}

void DrawButtons() {
	for (int i = 0; i < btnsCount; i++) {
		// Draw rect
		SetGraphicSetting(btns[i]->graphicData);
		CP_Graphics_DrawRect(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth);

		// Draw text
		SetTextSetting(btns[i]->textData);
		CP_Font_DrawText(btns[i]->textData.text, btns[i]->transform.x, btns[i]->transform.y);
	}
}

void SetGraphicSetting(GraphicData data) {
	CP_Settings_RectMode(data.imagePosMode);
	CP_Settings_ImageFilterMode(data.imageFilterMode);
	CP_Settings_Fill(data.color);
	CP_Settings_StrokeWeight(data.strokeWeigth);
}

void SetTextSetting(TextData data) {
	CP_Settings_TextSize(data.textSize);
	CP_Settings_Fill(data.color);
	CP_Settings_TextAlignment(data.hAlign, data.vAlign);
}

void CheckForButtonClick() {
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		Callback callBack = NULL;

		float xPos = CP_Input_GetMouseX();
		float yPos = CP_Input_GetMouseY();

		for (int i = 0; i < btnsCount; i++) {
			if (callBack != NULL) break;
			if (IsAreaClicked(btns[i]->graphicData.imagePosMode, btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, xPos, yPos)) {
				callBack = btns[i]->callBack;
			}
		}

		if (callBack != NULL) {
			callBack();
		}
	}
}

_Bool IsAreaClicked(CP_POSITION_MODE imagePosMode, float areaX, float areaY, float areaWidth, float areaHeigth, float clickX, float clickY)
{
	float halfWidth = areaWidth / 2;
	float halfHeigth = areaHeigth / 2;

	// If image is drawn from the corner, click detection will be different from image drawn from center.
	if (imagePosMode == CP_POSITION_CORNER) {
		if (clickX < areaX + areaWidth && clickX > areaX && clickY < areaY + areaHeigth && clickY > areaY) {
			return TRUE;
		}
	}
	// Image drawn from center.
	if (clickX > areaX - halfWidth && clickX < areaX + halfWidth && clickY < areaY + halfHeigth && clickY > areaY - halfHeigth) {
		return TRUE;
	}

	return FALSE;
}