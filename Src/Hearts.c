#include "cprocessing.h"
#include "GameOver.h"
#include "Hearts.h"

HeartContainer heart_stats[MAX_HEART_COUNT];
CP_Image heart_image;

// Initializes the life counter
// Remember spacing to be a value of above 1
void InitializeLife(void) {
	heart_image = CP_Image_Load("Assets/Heart.png");
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		heart_stats[i].heartAlive = 1; // Boolean True
		heart_stats[i].xpos = CP_System_GetDisplayWidth() / 10.f + IMAGE_HEART_LENGTH * HEART_SIZE * i + 25.f * i; // Spaces out the hearts by the size of the heart and adding the additional spacing
		heart_stats[i].ypos = CP_System_GetDisplayWidth() / 16.f - 50.f;
		heart_stats[i].size = HEART_SIZE;
		heart_stats[i].alpha = HEART_ALPHA;
		heart_stats[i].rotation = HEART_ROTATION;
	}
}

void GainLife(int gain_life) {
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		if (heart_stats[i].heartAlive == 0) {
			heart_stats[i].heartAlive = 1;
			heart_stats[i].alpha = 255;
			break;
		}
	}
	// TO DO : Add Gain Life Animation
}

void LoseLife(int lose_life) {
	int i = 0;
	if (i < MAX_HEART_COUNT && heart_stats[0].heartAlive != 0) {
		while (heart_stats[i].heartAlive == 1) {
			++i;
			continue;
		}
		heart_stats[i - 1].heartAlive = 0;
		heart_stats[i - 1].alpha = 0;
	}
	// TO DO : Add Lose Life Animation
}

void DrawLife(void) {
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		if (heart_stats[i].heartAlive == 1) {
			CP_Image_DrawAdvanced(heart_image, heart_stats[i].xpos, heart_stats[i].ypos, (float)IMAGE_HEART_LENGTH * heart_stats[i].size, (float)IMAGE_HEART_LENGTH * heart_stats[i].size, heart_stats[i].alpha, heart_stats[i].rotation);
		}
	}
}

// Check if total life = 0, if 0 bring to Game Over Screen
void CheckLoseCondition(void) {
	// Sum the total of heartAlive in heart_stats to total_life
	int total_life = 0;
	for (int i = 0; i < MAX_HEART_COUNT; ++i) {
		total_life += heart_stats[i].heartAlive;
	}
	if (total_life == 0) CP_Engine_SetNextGameState(GameOverInit, GameOverUpdate, GameOverExit);
}

void UpdateLife(void) {
	// DEBUGGING CHANGE TO THE GAIN LIFE AND LOSE LIFE CONDITIONS LATER
	if (CP_Input_KeyReleased(KEY_O) == 1) LoseLife(1);
	if (CP_Input_KeyReleased(KEY_P) == 1) GainLife(1);
	//Checks
	CheckLoseCondition();
	//Renders
	DrawLife();
}

void ClearHearts() {
	CP_Image_Free(&heart_image);
}
