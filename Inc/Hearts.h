#pragma once
#define MAX_HEART_COUNT 3
#define IMAGE_HEART_LENGTH 263
#define HEART_SPACING 0.35f
#define HEART_SIZE 4.f
#define HEART_ALPHA 255
#define HEART_ROTATION 0.f

typedef struct HeartContainer {
	int heartAlive;
	float xpos;
	float ypos;
	float size;
	int alpha;
	float rotation;
}HeartContainer;

typedef struct TextOnlyHearts {
	float font_size;
	float xpos;
	float ypos;
	CP_Color color;
	char* words;
} TextOnlyHearts;

typedef struct RectHearts {
	float xpos;
	float ypos;
	float height;
	float width;
} RectHearts;

void InitializeLife(void);
void GainLife(int gain_life);
void LoseLife(int lose_life);
void DrawLife(void);
int CheckLoseCondition(void);
void RenderHeartText(void);
float BezierCurve(float);
void AnimationLife(void);
void UpdateLife(void);
void ClearHearts(void);
