/*!
@file	  EnemyDisplay.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     09/11/2022
@brief    This header file
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "CProcessing.h"

typedef enum {
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
	TOP_LEFT
} E_DisplayCorner;

/*______________________________________________________________
@brief Change the positions of the enemy stats display here
*/
typedef enum {
	HEALTH = TOP_RIGHT,
	DAMAGE = BOTTOM_RIGHT
} E_DisplayType;

/*______________________________________________________________
@brief Calculates the offset for the corner displays based on the grid cell size
*/
void EnemyDisplayInit(void);

/*______________________________________________________________
@brief Renders the enemy stats

@param[in] pos - The position of the entity
@param[in] color - The color the text should be
@param[in] health - The health of the entity, set to negative number if should not be rendered
@param[in] wall_damage - The damage to walls the entity does, set to negative number if should not be rendered
*/
void RenderEnemyDisplay(float pos_x, float pos_y, CP_Color color, int health, int max_health, int wall_damage);

void RenderEnemyMovement(float pos_x, float pos_y, CP_Color color, int movement);
