#pragma once

#include "cprocessing.h"
#define HeaderFontSize 120.f
#define DefaultFontSize 48.f


//------------------------------
// Placeholder Text Maker
//------------------------------

// Making the struct of text
typedef struct TextOnly {
	float font_size;
	CP_Color color;
	char* words;
} TextOnly;

//-------------------------------

TextOnly CreateText(char* words, float size);
void RenderText(TextOnly Rendered_Text, float pos_x, float pos_y, CP_Color color);

void InitializeButtonsGameOverScreen(void);
void BackToMainMenu(void);

//-------------------------------

void GameOverInit();
void GameOverUpdate();
void GameOverExit();
