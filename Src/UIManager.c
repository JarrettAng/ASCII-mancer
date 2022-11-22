/*!
@file	  UIManager.c
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     21/11/2022
@brief    This source file for handling UI elements.
________________________________________________________________________________________________________*/

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

Text* textBoxes[MAX_UI_TEXTBOX];
int textBoxesCount = 0;

Slider* sliders[MAX_UI_SLIDERS];
int sliderCount = 0;
#pragma endregion

#pragma region UI_INTERACT_CACHE
Button* btnClicked = NULL;
Button* prevBtnClicked = NULL;

Button* btnHovered = NULL;
Button* prevBtnHovered = NULL;

Slider* sliderHeld = NULL;
Slider* prevSliderHeld = NULL;
#pragma endregion

#pragma region FORWARD_DECLARTIONS
void RenderButtons(void);
void RenderTexts(void);
void RenderTextBoxes(void);
void RenderSliders(Void);

void SetGraphicSetting(GraphicData data);
void SetTextSetting(TextData data);

void UpdateButtonClick(void);
void UpdateButtonHover(void);
void UpdateSliderHeld(void);

void FreeUI(void);
void FreeText(void);
void FreeButton(void);
void FreeSlider(void);
void ClearInteractCache(void);
#pragma endregion


#pragma region UPDATE_LOOP
void UIManagerUpdate(){
	// Render UI elements();
	RenderTexts();
	RenderTextBoxes();
	RenderButtons();
	RenderSliders();

	// Update UI intereaction cache.
	UpdateButtonClick();
	UpdateButtonHover();
	UpdateSliderHeld();
}
#pragma endregion

#pragma region INITIALIZE
// Intialize button with the given data.
void InitializeButton(Button* btn, Rect transform, GraphicData graphicsData, TextData textData, Callback callBack) {
	btn->transform = transform;
	// Initialized position.
	btn->transform.cachedPos = CP_Vector_Set(btn->transform.x, btn->transform.y);
	btn->graphicData = graphicsData;
	btn->textData = textData;
	btn->callBack = callBack;

	// Cache button.
	btns[btnsCount++] = btn;
}

// Intialize text with the given data.
void InitializeText(Text* txt, Rect transform, TextData data){
	txt->transform = transform;
	txt->transform.cachedPos = CP_Vector_Set(txt->transform.x, txt->transform.y);
	txt->textData = data;

	// Cache text.
	texts[textsCount++] = txt;
}

// Intialize text with the given data.
void InitializeTextBox(Text* txt, Rect transform, TextData data){
	txt->transform = transform;
	txt->transform.cachedPos = CP_Vector_Set(txt->transform.x, txt->transform.y);
	txt->textData = data;

	// Cache textbox.
	textBoxes[textBoxesCount++] = txt;
}

// Intialize slider with the given data.
void InitializeSlider(Slider* slider, Rect transform, Line line, CP_Image img, SliderKnob knobData){
	slider->transform = transform;
	slider->line = line;
	slider->img = img;
	slider->knob = knobData;

	// Cache slider.
	sliders[sliderCount++] = slider;
}
#pragma endregion

#pragma region RENDERING
// Require button to be intialized before rendering.
void RenderButton(Button* btn){
	// Draw rect
	SetGraphicSetting(btn->graphicData);
	CP_Graphics_DrawRect(btn->transform.x, btn->transform.y, btn->transform.width, btn->transform.heigth);

	// Draw text
	SetTextSetting(btn->textData);
	CP_Font_DrawText(btn->textData.text, btn->transform.x, btn->transform.y);

	if (btn->graphicData.img){
		CP_Image_Draw(btn->graphicData.img, btn->transform.x, btn->transform.y, btn->transform.width, btn->transform.heigth, 255);
	}
}

// Render all buttons cached in btns array.
void RenderButtons() {
	for (int i = 0; i < btnsCount; ++i) {
		// Draw rect
		SetGraphicSetting(btns[i]->graphicData);
		CP_Graphics_DrawRect(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth);

		if (btns[i]->textData.text != NULL){
			// Draw text
			SetTextSetting(btns[i]->textData);
			CP_Font_DrawText(btns[i]->textData.text, btns[i]->transform.x, btns[i]->transform.y);
		}

		if (btns[i]->graphicData.img){
			CP_Image_Draw(btns[i]->graphicData.img, btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, 255);
		}
	}
}

// Require text to be intialized before rendering.
void RenderText(Text* txt){
	// Draw text
	SetTextSetting(txt->textData);
	CP_Font_DrawText(txt->textData.text, txt->transform.x, txt->transform.y);
}

// Render all texts cached in texts array.
void RenderTexts(){
	// Loop through every text initialized.
	for (int i = 0; i < textsCount; ++i) {
		// Draw text
		SetTextSetting(texts[i]->textData);
		CP_Font_DrawText(texts[i]->textData.text, texts[i]->transform.x, texts[i]->transform.y);
	}
}

// Require text to be intialized before rendering.
void RenderTextBox(Text* txt){
	// Draw text
	SetTextSetting(txt->textData);
	CP_Font_DrawTextBox(txt->textData.text, txt->transform.x, txt->transform.y, txt->transform.width);
}

// Render all texts cached in texts array.
void RenderTextBoxes(){
	// Loop through every text boxes initialized.
	for (int i = 0; i < textBoxesCount; ++i) {
		// Draw text
		SetTextSetting(textBoxes[i]->textData);
		CP_Font_DrawTextBox(textBoxes[i]->textData.text, textBoxes[i]->transform.x, textBoxes[i]->transform.y, textBoxes[i]->transform.width);
	}
}

// Require slider to be intialized before rendering.
void RenderSlider(Slider* slider){
	CP_Image_Draw(slider->img, slider->transform.x, slider->transform.y, slider->transform.width, slider->transform.heigth, 255);
	CP_Image_Draw(slider->knob.img, slider->knob.transform.x, slider->knob.transform.y, slider->knob.transform.width, slider->knob.transform.heigth, 255);
}


// Render all sliders cached in slider array.
void RenderSliders(Void){
	// Loop through every slider initialized.
	for (int i = 0; i < sliderCount; ++i) {
		CP_Image_Draw(sliders[i]->img, sliders[i]->transform.x, sliders[i]->transform.y, sliders[i]->transform.width, sliders[i]->transform.heigth, 255);
		CP_Image_Draw(sliders[i]->knob.img, sliders[i]->knob.transform.x, sliders[i]->knob.transform.y, sliders[i]->knob.transform.width, sliders[i]->knob.transform.heigth, 255);
	}
}

// For updating engine graphic settings before drawing an UI element.
void SetGraphicSetting(GraphicData data) {
	CP_Settings_RectMode(data.imagePosMode);
	CP_Settings_ImageFilterMode(data.imageFilterMode);
	CP_Settings_Fill(data.color);
	CP_Settings_Stroke(data.strokeColor);
	CP_Settings_StrokeWeight(data.strokeWeight);
}


// For updating engine text settings before drawing an UI element.
void SetTextSetting(TextData data) {
	CP_Font_Set(data.font);
	CP_Settings_TextSize(data.textSize);
	CP_Settings_Fill(data.color);
	CP_Settings_TextAlignment(data.hAlign, data.vAlign);
}
#pragma endregion

#pragma region UI_INTERACTION
Button* GetBtnClicked(){
	return btnClicked;
}

Button* GetPrevBtnClicked(){
	return prevBtnClicked;
}

Button* GetBtnHovered(){
	return btnHovered;
}

Button* GetPrevBtnHovered(){
	return prevBtnHovered;
}

Slider* GetSliderHeld(){
	return sliderHeld;
}

Slider* GetPrevSliderHeld(){
	return prevSliderHeld;
}

// Triggers button's callback when player click on it.
void HandleButtonClick() {
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		float xPos = CP_Input_GetMouseX();
		float yPos = CP_Input_GetMouseY();

		// Loop through all buttons initialized in this scene.
		for (int i = 0; i < btnsCount; i++) {
			// Check if player is clicking a button.
			if (pointWithinArea(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, xPos, yPos, btns[i]->graphicData.imagePosMode)) {
				PlaySound(MOUSECLICK, CP_SOUND_GROUP_SFX);
				// If clciking a different button.
				if (btnClicked != btns[i]){
					// Cache prev button click.
					if (prevBtnClicked != btnClicked && btnClicked != NULL) prevBtnClicked = btnClicked;
					// Cache current button click.
					btnClicked = btns[i];
				}
				// Stop checking for buttons once a button is clicked.
				// Make sure the button has a callback event before breaking loop.
				// In an event where 2 buttons are overlapping, and one button doesnt have a callback, the other button callback will still be triggered.
				if (btnClicked != NULL && btnClicked->callBack != NULL) break;
			}
			// Player did not click on any button.
			btnClicked = NULL;
		}
		// Trigger onclick event if needed.
		if (btnClicked != NULL && btnClicked->callBack != NULL) {
			btnClicked->callBack();
		}
	}
}

// Cache the previous and current button clicked.
void UpdateButtonClick() {
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		float xPos = CP_Input_GetMouseX();
		float yPos = CP_Input_GetMouseY();

		// Loop through all buttons initialized in this scene.
		for (int i = 0; i < btnsCount; i++) {
			// Check if player is clicking a button.
			if (pointWithinArea(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, xPos, yPos, btns[i]->graphicData.imagePosMode)) {
				PlaySound(MOUSECLICK, CP_SOUND_GROUP_SFX);
				// If clciking a different button.
				if (btnClicked != btns[i]){
					// Cache prev button click.
					if (prevBtnClicked != btnClicked && btnClicked != NULL) prevBtnClicked = btnClicked;
					// Cache current button click.
					btnClicked = btns[i];
					return;
				}
			}
		}
		// Player did not click on any button.
		if (btnClicked != NULL) prevBtnClicked = btnClicked;
		btnClicked = NULL;
	}
}

// Cache the previous and current button hovered.
void UpdateButtonHover(){
	if (btnHovered != NULL && !CP_Input_MouseMoved()){
		// If already hovering and player did not move his mouse, just return to save resource.
		return btnHovered;
	}

	float xPos = CP_Input_GetMouseX();
	float yPos = CP_Input_GetMouseY();

	// Loop through all buttons initialized in this scene.
	for (int i = 0; i < btnsCount; i++) {
		// Check if player is hovering a button.
		if (pointWithinArea(btns[i]->transform.x, btns[i]->transform.y, btns[i]->transform.width, btns[i]->transform.heigth, xPos, yPos, btns[i]->graphicData.imagePosMode)) {
			// If hovering a different button.
			if (btnHovered != btns[i]){
				// Cache prev button hovered.
				if (prevBtnHovered != btnHovered && btnHovered != NULL) prevBtnHovered = btnHovered;
				// Cache current button hovered.
				btnHovered = btns[i];
			}
			return;
		}
	}
	// Cache last hovered button if player stop hovering.
	if (btnHovered != NULL) prevBtnHovered = btnHovered;
	// Player is not hovering on a button.
	btnHovered = NULL;
}

// Cache the previous and current slider held.
void UpdateSliderHeld(){
	if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)){
		// If player is already holding a slider, return it to save resources.
		// Only when player let go of the current slider, allow to hold another slider.
		if (sliderHeld != NULL){
			return sliderHeld;
		}

		float xPos = CP_Input_GetMouseX();
		float yPos = CP_Input_GetMouseY();

		// Loop through all buttons initialized in this scene.
		for (int i = 0; i < sliderCount; ++i) {
			// Check if mouse is within knob.
			if (pointWithinCircle(xPos, yPos, sliders[i]->knob.transform.x, sliders[i]->knob.transform.y, sliders[i]->knob.radius)){
				// If holding a different slider.
				if (sliderHeld != sliders[i]){
					// Cache prev slider held.
					if (prevSliderHeld != sliderHeld && sliderHeld != NULL) prevSliderHeld = sliderHeld;
					// Cache current slider held.
					sliderHeld = sliders[i];
					return;
				}
			}
		}
	}
	// Player let go of current held slider.
	if (CP_Input_MouseReleased(MOUSE_BUTTON_LEFT) && sliderHeld != NULL){
		// Cache last held slider.
		prevSliderHeld = sliderHeld;
		// Player is not holding a slider.
		sliderHeld = NULL;
	}
}
#pragma endregion

#pragma region MISC
// Clear all caches and free UI elements.
void FreeUIManager(){
	ClearInteractCache();
	FreeUI();
}

void ClearInteractCache(){
	btnClicked = NULL;
	prevBtnClicked = NULL;
	btnHovered = NULL;
	prevBtnHovered = NULL;
	sliderHeld = NULL;
	prevSliderHeld = NULL;
}

// Empty all UI arrays when exiting a scene, so that next scene can reuse the arrays.
void FreeUI(){
	FreeButton();
	FreeText();
	FreeSlider();
}

// Empty button array when exiting a scene, so that next scene can reuse the array.
void FreeButton(){
	// Empty array so next scene can use.
	memset(btns, 0, sizeof(btns));
	btnsCount = 0;
}

// Empty text array when exiting a scene, so that next scene can reuse the array.
void FreeText(){
	// Empty array so next scene can use.
	memset(texts, 0, sizeof(texts));
	textsCount = 0;
	memset(textBoxes, 0, sizeof(textBoxes));
	textBoxesCount = 0;
}

// Empty slider array when exiting a scene, so that next scene can reuse the array.
void FreeSlider(){
	// Empty array so next scene can use.
	memset(sliders, 0, sizeof(sliders));
	sliderCount = 0;
}
#pragma endregion
