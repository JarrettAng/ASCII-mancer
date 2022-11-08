#include "cprocessing.h"
#include "GameOver.h"
#include "Hearts.h"
#include "SoundManager.h"
#include "UIManager.h"
#include "ColorTable.h"

HeartContainer heart_stats[MAX_HEART_COUNT];
CP_Image heart_image;
TextOnlyHearts life_text;
TextOnlyHearts game_over_text;

// Initializes the life counter
// Remember spacing to be a value of above 1
void InitializeLife(void) {
	heart_image = CP_Image_Load("Assets/Heart.png");
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		heart_stats[i].heartAlive = 1; // Boolean True
		heart_stats[i].xpos = CP_System_GetDisplayWidth() / 5.f + 30.f + IMAGE_HEART_LENGTH * HEART_SIZE * i + 32.f * i; // Spaces out the hearts by the size of the heart and adding the additional spacing
		heart_stats[i].ypos = (CP_System_GetDisplayHeight() / 20.f) - 120.f;
		heart_stats[i].size = HEART_SIZE;
		heart_stats[i].alpha = HEART_ALPHA;
		heart_stats[i].rotation = HEART_ROTATION;
	}

	// Initialize text for life
	life_text.color = TETRIS_COLOR;
	life_text.font_size = 55.f;
	life_text.xpos = CP_System_GetDisplayWidth() / 12.f - 137.f;
	life_text.ypos = CP_System_GetDisplayHeight() / 16.f - 40.f;
	life_text.words = "HEARTS";
}

void EndTitleInit() {
	
}

//-----------------------
//  INPUTS
//-----------------------
void GainLife(int gain_life) {
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		if (heart_stats[i].heartAlive == 0) {
			heart_stats[i].heartAlive = 1;
			heart_stats[i].alpha = 255;
			break;
		}
	}
	PlaySound(GAINHEART, CP_SOUND_GROUP_SFX);
	// TO DO : Add Gain Life Animation
}

void LoseLife(int lose_life) {
	int i = 0;
	float total_flicker_time = 5.f;
	if (i < MAX_HEART_COUNT && heart_stats[0].heartAlive != 0) {
		while (heart_stats[i].heartAlive == 1) {
			++i;
		}
		heart_stats[i - 1].heartAlive = 0;
		heart_stats[i - 1].alpha = 0;
	}
	PlaySound(LOSE, CP_SOUND_GROUP_SFX);
	// TO DO : Add Lose Life Animation
}

//-----------------------
// CHECKS
//-----------------------

// Check if total life = 0, if 0 bring to Game Over Screen
void CheckLoseCondition(void) {
	// Sum the total of heartAlive in heart_stats to total_life
	int total_life = 0;
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		total_life += heart_stats[i].heartAlive;
	}
	if (total_life == 0) {
		PlaySound(GAMEOVER, CP_SOUND_GROUP_SFX);
		//CP_Engine_SetNextGameState(GameOverInit, GameOverUpdate, GameOverExit);
	}
}

//-----------------------
// RENDERS
//-----------------------
void DrawLife(void) {
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		if (heart_stats[i].heartAlive == 1) {
			CP_Image_DrawAdvanced(heart_image, heart_stats[i].xpos, heart_stats[i].ypos, (float)IMAGE_HEART_LENGTH * heart_stats[i].size, (float)IMAGE_HEART_LENGTH * heart_stats[i].size, heart_stats[i].alpha, heart_stats[i].rotation);
		}
	}
}

void RenderHeartText() {
	CP_Settings_Fill(life_text.color); // Color of text
	CP_Settings_TextSize(life_text.font_size); // Size of text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP); // Origin of text is it's absolute center
	CP_Font_DrawText(life_text.words, life_text.xpos, life_text.ypos); // Draw text
}

float BezierCurve(float t) {
	return t * t * (3.0f - 2.0f * t);
}

// AnimationLife variables
float time_elapsed = 0;
int up_tick_marker = 1;
int down_tick_marker = 0;
float duration = 0.75f; // 0.75 Seconds to loop

void AnimationLife() {
	float tick = time_elapsed / duration;
	// To check whether the heart needs to go down or up
	if (up_tick_marker == 1) time_elapsed += CP_System_GetDt();
	else if (down_tick_marker == 1) time_elapsed -= CP_System_GetDt();

	// Animate each heart
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		heart_stats[i].ypos = CP_Math_LerpFloat(CP_System_GetDisplayWidth() / 16.f - 60.f, (CP_System_GetDisplayWidth() / 16.f - 60.f) - 12.f, BezierCurve(tick));
	}
	// Check if it should be down ticking or up ticking
	if (time_elapsed >= duration) {
		down_tick_marker = 1;
		up_tick_marker = 0;
	}
	else if (time_elapsed <= 0) {
		up_tick_marker = 1;
		down_tick_marker = 0;
	}
	
}

//void EndTitleRender()

void UpdateLife(void) {
	// DEBUGGING CHANGE TO THE GAIN LIFE AND LOSE LIFE CONDITIONS LATER
	if (CP_Input_KeyReleased(KEY_O) == 1) LoseLife(1);
	if (CP_Input_KeyReleased(KEY_P) == 1) GainLife(1);
	//Checks
	CheckLoseCondition();
	//Renders
	RenderHeartText();
	DrawLife();

	AnimationLife();
}

void ClearHearts() {
	CP_Image_Free(&heart_image);
}
