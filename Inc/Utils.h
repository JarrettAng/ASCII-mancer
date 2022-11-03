#pragma once

// Base screen size.
// Aspect ratio: 16:9
#define BASE_WINDOW_WIDTH 1920
#define BASE_WINDOW_HEIGHT 1080

void UpdateUIScale(void);

float GetWindowWidth(void);
float GetWindowHeight(void);

float GetDisplayWidth(void);
float GetDisplayHeight(void);

float GetWidthScale(void);
float GetHeightScale(void);

typedef void(*Callback)(void);

// Struct to cache variables for a simple timer.
typedef struct Timer{
	float time;
	float elaspedTime;
}Timer;

_Bool pointWithinArea(float areaX, float areaY, float areaWidth, float areaHeigth, float xPos, float yPos, CP_POSITION_MODE areaMode);
_Bool pointWithinCircle(float xPos, float yPos, float circleX, float circleY, float radius);
