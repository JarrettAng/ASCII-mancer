#pragma once
#include <cprocessing.h>            //CP_Vector, CP_Random_Noise

/*____________________________________________________________________*/

/*
@brief Value clamped to 0-1, increment it to induce screenshaking
*/
extern float trauma;
/*____________________________________________________________________*/

/*
@brief Returns a random float with range -1 to 1 based off of a seed.
@param[in] seed to set the Perlin noise to 
*/
float GetPerlinFloat(float seed);   
/*____________________________________________________________________*/

/*
@brief Returns a random vector using perlin floats. XY values will be between -1 and 1
*/
CP_Vector GetRandomVector(void);    
/*____________________________________________________________________*/

/*
@brief Updates the shaking of the canvas. Must include in update to work. NOTE: Canvas must ALWAYS BE UPDATED FIRST before anything else if not there will be NO movement!!!
*/
void UpdateCameraShaker(void);
/*____________________________________________________________________*/

/*
@brief Stops the camera from shaking by resetting trauma. (Used when exiting to mainmenu from game)
*/
void ResetCameraShake(void);

