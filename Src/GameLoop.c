/*!
@file	  GameLoop.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     27/10/2022
@brief    This source file 
________________________________________________________________________________________________________*/

#include "GameLoop.h"

TurnType current_turn_type;
Turn *current_turn;

//______________________________________________________________
// Event subscriptions arrays (E.g. Player turn start, Zombie turn end)
Turn player_turn;
Turn zombie_turn;

#pragma region

#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the GameLoop.h)

/*______________________________________________________________
@brief Needs to be called at the start of the game level to handle the turns between
	   the player and the zombies
*/
void GameLoopInit(void) {
	// Set everything just in case
	UnsubscribeAllEvents();

	// Set the turn to start as player
	current_turn_type = TURN_PLAYER;
	current_turn = &player_turn;
	// Call the start events for the current turn
	int total_events_subscribed = current_turn->start.count;
	for (int index = 0; index < total_events_subscribed; ++index) {
		current_turn->start.events[index]();
	}
}

/*______________________________________________________________
@brief Switches the turn state, calls the end events for the current state and the start events for the new one.
*/
void GameLoopSwitch(TurnType new_turn) {
	// Call the exit events for the current turn
	int total_events_subscribed = current_turn->end.count;
	for (int index = 0; index < total_events_subscribed; ++index) {
		current_turn->end.events[index]();
	}

	// Update the current turn to the new one
	current_turn_type = new_turn;
	switch (current_turn_type) {
	case TURN_PLAYER: current_turn = &player_turn; break;
	case TURN_ZOMBIE: current_turn = &zombie_turn; break;
	}

	// Call the start events for the current turn
	total_events_subscribed = current_turn->start.count;
	for (int index = 0; index < total_events_subscribed; ++index) {
		current_turn->start.events[index]();
	}
}

/*______________________________________________________________
@brief Every update, calls all functions subscribed to the current turn's update.
*/
void GameLoopUpdate(void) {
	int total_events_subscribed = current_turn->update.count;
	for (int index = 0; index < total_events_subscribed; ++index) {
		current_turn->update.events[index]();
	}
}

//______________________________________________________________
// All subscription functions to all events
void Subscribe_PlayerTurnStart(void(*function_pointer)(void)) {
	player_turn.start.events[player_turn.start.count] = function_pointer;
	++player_turn.start.count;
}

void Subscribe_PlayerTurnUpdate(void(*function_pointer)(void)) {
	player_turn.update.events[player_turn.update.count] = function_pointer;
	++player_turn.update.count;
}

void Subscribe_PlayerTurnEnd(void(*function_pointer)(void)) {
	player_turn.end.events[player_turn.end.count] = function_pointer;
	++player_turn.end.count;
}

void Subscribe_ZombieTurnStart(void(*function_pointer)(void)) {
	zombie_turn.start.events[zombie_turn.start.count] = function_pointer;
	++zombie_turn.start.count;
}

void Subscribe_ZombieTurnUpdate(void(*function_pointer)(void)) {
	zombie_turn.update.events[zombie_turn.update.count] = function_pointer;
	++zombie_turn.update.count;
}

void Subscribe_ZombieTurnEnd(void(*function_pointer)(void)) {
	zombie_turn.end.events[zombie_turn.end.count] = function_pointer;
	++zombie_turn.end.count;
}

/*______________________________________________________________
@brief It is important this function is called when the game level exits!!
*/
void UnsubscribeAllEvents(void) {
	int total_events_subscribed;
	
	// Remove references to all events to player start, if any
	if (total_events_subscribed = player_turn.start.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			player_turn.start.events[index] = NULL;
		}
	}
	// Remove references to all events to player update, if any
	if (total_events_subscribed = player_turn.update.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			player_turn.update.events[index] = NULL;
		}
	}
	// Remove references to all events to player end, if any
	if (total_events_subscribed = player_turn.end.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			player_turn.end.events[index] = NULL;
		}
	}
	// Remove references to all events to zombie start, if any
	if (total_events_subscribed = zombie_turn.start.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			zombie_turn.start.events[index] = NULL;
		}
	}
	// Remove references to all events to zombie update, if any
	if (total_events_subscribed = zombie_turn.update.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			zombie_turn.update.events[index] = NULL;
		}
	}
	// Remove references to all events to zombie end, if any
	if (total_events_subscribed = zombie_turn.end.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			zombie_turn.end.events[index] = NULL;
		}
	}

	// Reset the counters to 0 as well
	player_turn.start.count = 0;
	player_turn.update.count = 0;
	player_turn.end.count = 0;

	zombie_turn.start.count = 0;
	zombie_turn.update.count = 0;
	zombie_turn.end.count = 0;
}
