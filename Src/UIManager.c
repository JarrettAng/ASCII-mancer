#include <cprocessing.h>
#include "UIManager.h"
#include "Utils.h"
#include "ColorTable.h"
#include "SoundManager.h"

#pragma region UI_CACHE

Button* btns[MAX_UI_BUTTONS];
int btnsCount = 0;

Text* texts[MAX_UI_TEXT];
int textsCount = 0;

Slider* sliders[MAX_UI_SLIDERS];
int sliderCount = 0;

#pragma endregion

#pragma region UI_INTERACT_CACHE

Button* btnClicked = NULL;
Button* btnHovered = NULL;
Button* prevBtnHovered = NULL;

#pragma endregion

#pragma region INITIALIZE

/// <summary>
/// Intialize button with the given data. Also store btn address in array so it manager can draw them all at once.
/// </summary>
void InitializeButton(Button* btn, Rect transform, GraphicData graphicsData, TextData textData, Callback callBack) {
	btn->transform = transform;
	// Initialized position.
	btn->transform.cachedPos = CP_Vector_Set(btn->transform.x, btn->transform.y);
	btn->graphicData = graphicsData;
	btn->textData = textData;
	btn->callBack = callBack;

	btns[btnsCount++] = btn;
}

/// <summary>
/// Intialize text with the given data.Also store text address in array so it manager can draw them all at once.
/// </summary>
void InitializeText(Text* txt, Rect transform, TextData data){
	txt->transform = transform;
	txt->transform.cachedPos = CP_Vector_Set(txt->transform.x, txt->transform.y);
	txt->textData = data;

	texts[textsCount++] = txt;
}

void InitializeSlider(Slider* slider, Rect transform, Line line, CP_Image img, SliderKnob knobData){
	slider->transform = transform;
	slider->line = line;
	slider->img = img;
	slider->knob = knobData;
	sliders[sliderCount++] = slider;
}

#pragma endregion

#pragma region RENDERING

/// <summary>
/// Require button to be intialized before drawing.
/// </summary>
void DrawButton(Button* btn){
	// Draw rect
	SetGraphicSetting(btn->graphicData);
	CP_Graphics_DrawRect(btn->transform.x, btn->transform.y, btn->transform.width, btn->transform.heigth);

	// Draw text
	SetTextSetting(btn->textData);
	CP_Font_DrawText(btn->textData.text, btn->transform.x, btn->transform.y);
}

/// <summary>
/// Draw all buttons cached in btns array.
/// </summary>
void DrawButtons() {
	for (int i = 0; i < btnsCount; ++i) {
		// Draw rect
		SetGraphicSetting(btns[i]->graphicData);
		CP_Graphics_DrawRect(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth);

		// Draw text
		SetTextSetting(btns[i]->textData);
		CP_Font_DrawText(btns[i]->textData.text, btns[i]->transform.x, btns[i]->transform.y);
	}
}

/// <summary>
/// Require text to be intialized before drawing.
/// </summary>
//void RenderText(Text* txt){
//	// Draw text
//	SetTextSetting(txt->textData);
//	CP_Font_DrawText(txt->textData.text, txt->transform.x, txt->transform.y);
//}

/// <summary>
/// Draw all buttons cached in texts array.
/// </summary>
void RenderTexts(){
	for (int i = 0; i < textsCount; ++i) {
		// Draw text
		SetTextSetting(texts[i]->textData);
		CP_Font_DrawText(texts[i]->textData.text, texts[i]->transform.x, texts[i]->transform.y);
	}
}

void RenderSliders(Void){
	for (int i = 0; i < sliderCount; ++i) {
		CP_Image_Draw(sliders[i]->img, sliders[i]->transform.x, sliders[i]->transform.y, sliders[i]->transform.width, sliders[i]->transform.heigth, 255);
		CP_Image_Draw(sliders[i]->knob.img, sliders[i]->transform.x, sliders[i]->transform.y, sliders[i]->knob.transform.width, sliders[i]->knob.transform.heigth, 255);
	}
}

/// <summary>
/// For updating engine graphic settings before drawing an UI element.
/// </summary>
void SetGraphicSetting(GraphicData data) {
	CP_Settings_RectMode(data.imagePosMode);
	CP_Settings_ImageFilterMode(data.imageFilterMode);
	CP_Settings_Fill(data.color);
	CP_Settings_Stroke(data.strokeColor);
	CP_Settings_StrokeWeight(data.strokeWeight);
}


/// <summary>
/// For updating engine text settings before drawing an UI element.
/// </summary>
void SetTextSetting(TextData data) {
	CP_Font_Set(data.font);
	CP_Settings_TextSize(data.textSize);
	CP_Settings_Fill(data.color);
	CP_Settings_TextAlignment(data.hAlign, data.vAlign);
}

#pragma endregion

#pragma region UI_INTERACTION

Button* GetPrevBtnHovered(){
	return prevBtnHovered;
}

Button* GetBtnClicked(){
	return btnClicked;
}

/// <summary>
/// Triggers button's callback when player click on it.
/// </summary>
void HandleButtonClick() {
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		//Button* btnClicked = NULL;

		float xPos = CP_Input_GetMouseX();
		float yPos = CP_Input_GetMouseY();

		// Loop through all buttons initialized in this scene.
		for (int i = 0; i < btnsCount; i++) {
			// Check if player is clicking a button.
			if (pointWithinArea(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, xPos, yPos, btns[i]->graphicData.imagePosMode)) {
				// Cache callback to be triggered after the conditional statement.
				// Will cause error if callback is triggered here.
				btnClicked = btns[i];
				// Stop checking for buttons once a button is clicked.
				// Make sure the button has a callback event before breaking loop.
				// In an event where 2 buttons are overlapping, and one button doesnt have a callback, the other button callback will still be triggered.
				if (btnClicked->callBack != NULL) break;
			}
		}
		// Trigger onclick event if needed.
		if (btnClicked != NULL && btnClicked->callBack != NULL) {
			btnClicked->callBack();
		}
	}
}

/// <summary>
/// Get the address of the button the player clicked.
/// </summary>
Button* GetButtonClick() {
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		float xPos = CP_Input_GetMouseX();
		float yPos = CP_Input_GetMouseY();

		// Loop through all buttons initialized in this scene.
		for (int i = 0; i < btnsCount; i++) {
			// Check if player is clicking a button.
			if (pointWithinArea(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, xPos, yPos, btns[i]->graphicData.imagePosMode)) {
				PlaySound(MOUSECLICK, CP_SOUND_GROUP_SFX);
				btnClicked = btns[i];
				return btns[i];
			}
		}
		return NULL;
	}
	return NULL;
}

/// <summary>
/// Returns the button the player is hovering over.
/// </summary>
Button* GetButtonHover(){
	float xPos = CP_Input_GetMouseX();
	float yPos = CP_Input_GetMouseY();

	// Loop through all buttons initialized in this scene.
	for (int i = 0; i < btnsCount; i++) {
		// Check if player is hovering a button.
		if (pointWithinArea(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, xPos, yPos, btns[i]->graphicData.imagePosMode)) {
			if (btnHovered != btns[i]){
				// Cached previous button if player is hovering on a new button.
				if (btnHovered != NULL) prevBtnHovered = btnHovered;
				btnHovered = btns[i];
			}
			// Break loop and return the button the player is hovering.
			return btns[i];
		}
	}
	// Cache last hovered button if player stop hovering.
	if (btnHovered != NULL) prevBtnHovered = btnHovered;
	// Player is not hovering on a button.
	btnHovered = NULL;
	return NULL;
}

SliderKnob* GetSliderKnobHeld(){
	if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)){
		float xPos = CP_Input_GetMouseX();
		float yPos = CP_Input_GetMouseY();

		// Loop through all buttons initialized in this scene.
		for (int i = 0; i < sliderCount; i++) {
			if (pointWithinCircle(xPos, yPos, sliders[i]->knob.transform.x, sliders[i]->knob.transform.y, sliders[i]->knob.radius)){
				return &sliders[i]->knob;
			}
		}
	}
	return NULL;
}

#pragma endregion

void ClearInteractCache(){
	btnClicked = NULL;
	btnHovered = NULL;
	prevBtnHovered = NULL;
}

/// <summary>
/// Empty all UI arrays when exiting a scene, so that next scene can reuse the arrays.
/// </summary>
void FreeUI(){
	FreeButton();
	FreeText();
}

/// <summary>
/// Empty button array when exiting a scene, so that next scene can reuse the array.
/// </summary>
void FreeButton(){
	// Empty array so next scene can use.
	memset(btns, 0, sizeof(btns));
	btnsCount = 0;
}

/// <summary>
/// Empty text array when exiting a scene, so that next scene can reuse the array.
/// </summary>
void FreeText(){
	// Empty array so next scene can use.
	memset(texts, 0, sizeof(texts));
	textsCount = 0;
}
