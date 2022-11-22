/*!
@file	  GameLoop.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@date     17/11/2022
@brief    This header file contains enums for turn types (player, zombies) and event types (start, update, end) for those turns,
		  structs to hold information (pointers, priority) for the events and an array of events for each turn, and 7 functions.

		  SubscribeEvent - Add a function to be called everytime an event happens (e.g. player start, zombie update).
		  UnsubscribeEvent - Pass the event you want to unsubscribe your function from, and pass the function pointer too.
		  UnsubscribeAllEvents - It is important this function is called when the game level exits!!
		  ZeroOutAllEvents - Similar to UnsubscribeAllEvents but this forces ALL events to be zeroed out, even the NULL ones

		  GameLoopInit - Needs to be called at the start of the game level to handle the turns between the player and the zombies.
		  GameLoopSwitch - Call this to swap between turns. The functions subscribed to the end of current turn and start of next turn will be called.
		  GameLoopUpdate - Every update, calls all functions subscribed to the turn's update array.
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
/*______________________________________________________________
@brief The different types of events you can subscribe to
*/
typedef enum {
	PLAYER_START,
	PLAYER_UPDATE,
	PLAYER_END,
	ZOMBIE_START,
	ZOMBIE_UPDATE,
	ZOMBIE_END
} TurnEventType;
#define DEFAULT_PRIORITY 0 

/*______________________________________________________________
@brief     If you need a function called during any of the player's or zombie's turn, pass your function here.
	       ONLY VOID FUNCTION(VOID) (Functions that return nothing and have no parameters) allowed.

@functions TYPE - See TurnEventType enum (Player & Zombie)
		   START - Called when first swapping to this turn
	       UPDATE - Called every update during this turn
	       END - Called when swapping out to another turn

@priority  !! For Priority, higher numbers will be called first, the DEFAULT_PRIORITY int is 0 use it if you have no preference. 
			  If your function needs to be called first, use a higher number (not too high) like 5 or 20, if your function needs
			  to be called last, use a lower number (negative) like -1 or -20.
*/
void SubscribeEvent(TurnEventType event_type, void(*function_pointer)(void), int priority);

/*______________________________________________________________
@brief Pass the event you want to unsubscribe your function from, and pass the function pointer too so it knows which one to remove.

@param[in] event_type - The type of event to subscribe to
@param[in] function_pointer - The pointer to the function to subscribe
*/
void UnsubscribeEvent(TurnEventType event_type, void(*function_pointer)(void));

/*______________________________________________________________
@brief It is important this function is called when the game level exits!!
*/
void UnsubscribeAllEvents(void);

/*______________________________________________________________
@brief Similar to UnsubscribeAllEvents but this forces ALL events to be zeroed out, even the NULL ones
*/
void ZeroOutAllEvents(void);

//______________________________________________________________
// Turn & Events structs

/*______________________________________________________________
@brief Each event stores a pointer and the priority in event list array
*/
typedef struct {
	void (*event)(void);
	int priority;
} Event;

/*______________________________________________________________
@brief Each event array can store up to MAX_EVENTS_BUFFER events, stores the total subscribed count as well
*/
#define MAX_EVENTS_BUFFER 10
typedef struct {
	Event events[MAX_EVENTS_BUFFER];
	int count;
} TurnEvent;

/*______________________________________________________________
@brief Each turn has 3 event arrays that can be subscribed to, the start, update, and end of a turn
*/
typedef struct {
	TurnEvent start;
	TurnEvent update;
	TurnEvent end;
} Turn;

/*______________________________________________________________
@brief The identifiers for the all possible turn types
*/
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
@brief Call this to swap between turns. The functions subscribed to the end of current turn and start of next turn will be called.

@param[in] new_turn - The turn type to swap to next
*/
void GameLoopSwitch(TurnType new_turn);

/*______________________________________________________________
@brief Every update, calls all functions subscribed to the turn's update array.
*/
void GameLoopUpdate(void);
