#include "Settings.h"
#include "ColorTable.h"
#include "Utils.h"
#include "UIManager.h"


Slider volumeSlider;
SliderKnob volumeKnob;


#pragma region FORWARD_DECLARATIONS

void DrawSlider(void);
void DrawSliderBob(void);

#pragma endregion


void SettingsInit(void){

	Slider sliderData = {
	.start = CP_Vector_Set(500, 500),
	.end = CP_Vector_Set(1000, 500),

	.stroke = MENU_RED,
	.strokeWeight = 10
	};

	SliderKnob knobData = {
	.x = sliderData.start.x,
	.y = sliderData.start.y,
	.bobRadius = 25,

	.colour = WHITE,
	.stroke = MENU_RED,
	.strokeWeight = 10
	};

	volumeKnob = knobData;
	volumeSlider = sliderData;

}

void SettingsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);

	DrawSlider();
	DrawSliderBob();

}

void SettingsExit(void){
}

void DrawSlider(){
	CP_Settings_Stroke(volumeSlider.stroke);
	CP_Settings_StrokeWeight(volumeSlider.strokeWeight);

	CP_Graphics_DrawLine(volumeSlider.start.x, volumeSlider.start.y, volumeSlider.end.x, volumeSlider.end.y);
}

void DrawSliderBob(){
	CP_Settings_Fill(volumeKnob.colour);
	CP_Settings_Stroke(volumeKnob.stroke);
	CP_Settings_StrokeWeight(volumeKnob.strokeWeight);

	CP_Graphics_DrawCircle(CP_Math_ClampFloat(volumeKnob.x, volumeSlider.start.x, volumeSlider.end.x), CP_Math_ClampFloat(volumeKnob.y, volumeSlider.start.y, volumeSlider.end.y), volumeKnob.bobRadius * 2);
}
