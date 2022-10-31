/*!
@file	  GameLoop.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     27/10/2022
@brief    This header file 
________________________________________________________________________________________________________*/

#pragma once // Only include this header file once

/*
	______                 __		  	       __                   _       __  _
   / ____/   _____  ____  / /_	   _______  __/ /_  _______________(_)___  / /_(_)___  ____  _____
  / __/ | | / / _ \/ __ \/ __/	  / ___/ / / / __ \/ ___/ ___/ ___/ / __ \/ __/ / __ \/ __ \/ ___/
 / /___ | |/ /  __/ / / / /_	 (__  ) /_/ / /_/ (__  ) /__/ /  / / /_/ / /_/ / /_/ / / / (__  )
/_____/ |___/\___/_/ /_/\__/	/____/\__,_/_.___/____/\___/_/  /_/ .___/\__/_/\____/_/ /_/____/
																 /_/						Read below \/
*/
#define DEFAULT_PRIORITY 0 
/*______________________________________________________________
@brief PLAYER'S TURN EVENTS, if you need a function called during this turn, pass your function here.
	   ONLY VOID FUNCTION(VOID) (Functions that return nothing and have no parameters) allowed.
	   START - Called when first swapping to this turn
	   UPDATE - Called every update during this turn
	   END - Called when swapping out to another turn
	   !! For Priority, higher numbers will be called first, the DEFAULT_PRIORITY int is 0. If your function
	      needs to be called first, use a higher number (not too high) like 5 or 20, if your function needs to
		  be called last, use a lower number (negative) like -1 or -20.
*/
void Subscribe_PlayerTurnStart(void(*function_pointer)(void), int priority);

void Subscribe_PlayerTurnUpdate(void(*function_pointer)(void), int priority);

void Subscribe_PlayerTurnEnd(void(*function_pointer)(void), int priority);

/*______________________________________________________________
@brief ZOMBIE'S TURN EVENTS, if you need a function called during this turn, pass your function here.
	   ONLY VOID FUNCTION(VOID) (Functions that return nothing and have no parameters) allowed.
	   START - Called when first swapping to this turn
	   UPDATE - Called every update during this turn
	   END - Called when swapping out to another turn
	   !! For Priority, higher numbers will be called first, the DEFAULT_PRIORITY int is 0. If your function
		  needs to be called first, use a higher number (not too high) like 5 or 20, if your function needs to
		  be called last, use a lower number (negative) like -1 or -20.
*/
void Subscribe_ZombieTurnStart(void(*function_pointer)(void), int priority);

void Subscribe_ZombieTurnUpdate(void(*function_pointer)(void), int priority);

void Subscribe_ZombieTurnEnd(void(*function_pointer)(void), int priority);

/*______________________________________________________________
@brief It is important this function is called when the game level exits!!
*/
void UnsubscribeAllEvents(void);

typedef struct {
	void (*event)(void);
	int priority;
} Event;

#define MAX_EVENTS_BUFFER 10
typedef struct {
	Event events[MAX_EVENTS_BUFFER];
	int count;
} TurnEvent;

typedef struct {
	TurnEvent start;
	TurnEvent update;
	TurnEvent end;
} Turn;

typedef enum {
	TURN_PLAYER,
	TURN_ZOMBIE
} TurnType;

#define NULL ((void *)0) // Null pointer, for unsubscribing events

/*______________________________________________________________
@brief Needs to be called at the start of the game level to handle the turns between the player and the zombies.
*/
void GameLoopInit(void);

/*______________________________________________________________
@brief Call this every time the turn swaps to call all the functions subscribed to the turn start and end arrays.
*/
void GameLoopSwitch(TurnType new_turn);

/*______________________________________________________________
@brief Every update, calls all functions subscribed to the turn's update array.
*/
void GameLoopUpdate(void);
