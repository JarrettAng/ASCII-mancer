/*!
@file	  EnemyDisplay.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     17/11/2022
@brief    This header file contains an enum to reference the 4 corners of the cell used for rendering enemy stats,
		  structs to hold information to render different stats and 5 functions,

		  EnemyDisplayInit - Calculates the offset for the corner displays based on the grid cell size

		  RenderEnemyDisplay - Renders the enemy stats in the cell it is in (health & wall damage)
		  RenderEnemyMovement - Renders the enemy movement arrows and shades destination cell
		  DisplayEnemyInfo - Renders the floating information box on enemy hover

		  FreeEnemyDisplayIcon - Frees the attack icons used for enemy attack stat displaying
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

#include "CProcessing.h" // For CP_Color & CP_Vector
#include "EnemyStats.h" // For enemy type enum

/*______________________________________________________________
@brief Anchors for the four corners of the enemy cell
*/
typedef enum {
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
	TOP_LEFT
} E_DisplayCorner;

/*______________________________________________________________
@brief Anchors for the four corners of the enemy cell
*/
typedef struct {
	EnemyInfo *type;
	char *description;
	int char_count;
} E_DisplayInfo;

/*______________________________________________________________
@brief Struct for the show enemy information on hover
*/
typedef struct {
	CP_Vector offset;
	float size;
	float stroke;
	ZombieType zombie_index;
} E_DisplayText;

/*______________________________________________________________
@brief Calculates the offset for the corner displays based on the grid cell size
*/
void EnemyDisplayInit(void);

/*______________________________________________________________
@brief Renders the enemy stats in the cell it is in (health & wall damage)

@param[in] pos - The position of the entity
@param[in] health - The current health of the entity, set to 0 if should not be rendered
@param[in] health - The maximum health of the entity
@param[in] wall_damage - The damage to walls the entity does, set to 0 if should not be rendered
*/
void RenderEnemyDisplay(float pos_x, float pos_y, int health, int max_health, int wall_damage);

/*______________________________________________________________
@brief Renders the enemy movement arrows and shades destination cell

@param[in] pos_x - The screen x position of the entity
@param[in] pos_y - The screen y position of the entity
@param[in] movement - The movement speed of the entity
*/
void RenderEnemyMovement(float pos_x, float pos_y, int movement);

/*______________________________________________________________
@brief Renders the floating information box on enemy hover
*/
void DisplayEnemyInfo(void);

/*______________________________________________________________
@brief Frees the attack icons used for enemy attack stat displaying
*/
void FreeEnemyDisplayIcon(void);
