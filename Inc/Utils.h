#pragma once

// Base screen size.
// Aspect ratio: 16:9
#define BASE_WINDOW_WIDTH 1920
#define BASE_WINDOW_HEIGHT 1080

void UpdateUIScale(void);

float GetWindowWidth(void);
float GetWindowHeight(void);

float GetWidthScale(void);
float GetHeightScale(void);

typedef void(*Callback)(void);

// Struct to cache variables for a simple timer.
typedef struct Timer{
	float time;
	float elaspedTime;
}Timer;