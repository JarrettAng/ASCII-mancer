#include <cprocessing.h>

#define WHITE CP_Color_Create(255, 255, 255, 255)
#define BLACK CP_Color_Create(0, 0, 0, 255)
#define GREEN CP_Color_Create(0, 255, 0, 255)
#define TRANSPERANT CP_Color_Create(0,0,0,0)

#define TRANSPERANT_CODE 0,0,0,0

/*==============*/
/*	MENU COLORS	*/
/*==============*/
#define MENU_BLACK CP_Color_Create(35, 35, 35, 255)
#define MENU_GRAY CP_Color_Create(179, 179, 179, 255)
#define MENU_RED CP_Color_Create(227, 23, 23, 255)
#define MENU_WHITE CP_Color_Create(239, 239, 239, 255)

#define MENU_RED_CODE 227, 23, 23, 255

/*======================*/
/*	GAME OVER COLORS	*/
/*======================*/
#define GAMEOVER_RED CP_Color_Create(175, 100, 100, 255)
#define GAMEOVER_DARKGRAY CP_Color_Create(25, 25 ,25 ,255)

/*====================*/
/*	GAME LEVEL PICES  */
/*====================*/
#define GRID_COLOR CP_Color_Create(72, 72, 72, 255)
#define TETRIS_COLOR CP_Color_Create(197, 197, 197, 255)
#define TETRIS_SLOT_COLOR CP_Color_Create(66, 66, 66, 255)
#define TETRIS_SLOT_WALL_COLOR CP_Color_Create(197, 197, 197, 255)
#define TETRIS_SLOT_ATTACK_COLOR CP_Color_Create(227, 23, 23, 255)
#define TETRIS_HOVER_COLOR CP_Color_Create(197, 197, 197, 130)
