#pragma once

#include "cprocessing.h"
#define HeaderFontSize 120.f
#define DefaultFontSize 48.f


//------------------------------
// Placeholder Text Maker
//------------------------------

// Making the struct of text
typedef struct Text {
	float font_size;
	CP_Color color;
	char* words;
} Text;

// Making the struct of a button
typedef struct Button {
	float width;
	float height;
	float pos_x;
	float pos_y;
	CP_Color color;
	Text text;
} Button;

//-------------------------------

Text CreateText(char* words, float size);
void RenderText(Text Rendered_Text, float pos_x, float pos_y, CP_Color color);
Button CreateButtonWithText(Text text, float width, float height, float pos_x, float pos_y);
void RenderButtonWithText(Button Rendered_Button, CP_Color color, CP_Color stroke_color, float stroke_weight, CP_Color text_color);
int IsButtonPressed(Button Button_Check);

//-------------------------------

void GameOverInit();
void GameOverUpdate();
void GameOverExit();
