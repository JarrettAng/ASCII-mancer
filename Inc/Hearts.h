#pragma once
#define MAX_HEART_COUNT 3
#define IMAGE_HEART_LENGTH 263
#define HEART_SIZE 0.25f
#define HEART_ALPHA 255
#define HEART_ROTATION 0.f

typedef struct HeartContainer {
	int heartAlive;
	float xpos;
	float ypos;
	float size;
	float alpha;
	float rotation;
}HeartContainer;

void InitializeLife(void);
void GainLife(int gain_life);
void LoseLife(int lose_life);
void DrawLife(void);
void UpdateLife(void);
void CheckLoseCondition(void);
void ClearHearts(void);
