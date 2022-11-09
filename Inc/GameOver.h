#pragma once

#include "cprocessing.h"
#define HEADER_FONT_SIZE 120.f
#define DEFAULT_FONT_SIZE 48.f


//------------------------------
// Placeholder Text Maker
//------------------------------

// Making the struct of text
typedef struct TextOnly {
	float font_size;
	float xpos;
	float ypos;
	CP_Color color;
	char* words;
} TextOnly;

//-------------------------------

TextOnly CreateText(char* words, float font_size, CP_Color color, float xpos, float ypos);
void RenderGOText(TextOnly Rendered_Text);
void InitializeAllText(void);
void RenderAllText(void);

void InitializeButtonsGameOverScreen(void);
void BackToMainMenu(void);

//-------------------------------

void GameOverInit();
void GameOverUpdate();
void GameOverExit();
