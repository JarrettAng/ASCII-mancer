#pragma once
#include <cprocessing.h>            //CP_Vector, CP_Random_Noise


extern float trauma;                //Value clamped to 0-1, increment it to induce screenshaking

float GetPerlinFloat(float seed);   //Returns a random float with range -1 to 1 based off of a seed.

CP_Vector GetRandomVector(void);    //Returns a random vector using perlin floats. XY values will be between -1 and 1

//NOTE: Canvas must ALWAYS BE UPDATED FIRST before anything else if not there will be NO movement!!!
void UpdateCameraShaker(void);      //Function that updates the shaking of the canvas. Must include in update to work.