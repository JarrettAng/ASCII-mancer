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
void InsertToArrayAt(TurnEvent* turn_array, void(*function_pointer)(void), int priority);
#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the GameLoop.h)

/*______________________________________________________________
@brief Needs to be called at the start of the game level to handle the turns between
	   the player and the zombies
*/
void GameLoopInit(void) {
	// Set the turn to start as player
	current_turn_type = TURN_PLAYER;
	current_turn = &player_turn;
	// Call the start events for the current turn
	int total_events_subscribed = current_turn->start.count;
	for (int index = 0; index < total_events_subscribed; ++index) {
		current_turn->start.events[index].event();
	}
}

/*______________________________________________________________
@brief Switches the turn state, calls the end events for the current state and the start events for the new one.
*/
void GameLoopSwitch(TurnType new_turn) {
	// Call the exit events for the current turn
	int total_events_subscribed = current_turn->end.count;
	for (int index = 0; index < total_events_subscribed; ++index) {
		current_turn->end.events[index].event();
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
		current_turn->start.events[index].event();
	}
}

/*______________________________________________________________
@brief Every update, calls all functions subscribed to the current turn's update.
*/
void GameLoopUpdate(void) {
	int total_events_subscribed = current_turn->update.count;
	for (int index = 0; index < total_events_subscribed; ++index) {
		current_turn->update.events[index].event();
	}
}

//______________________________________________________________
// All subscription functions to all events
void Subscribe_PlayerTurnStart(void(*function_pointer)(void), int priority) {
	InsertToArrayAt(&player_turn.start, function_pointer, priority);
}

void Subscribe_PlayerTurnUpdate(void(*function_pointer)(void), int priority) {
	InsertToArrayAt(&player_turn.update, function_pointer, priority);
}

void Subscribe_PlayerTurnEnd(void(*function_pointer)(void), int priority) {
	InsertToArrayAt(&player_turn.end, function_pointer, priority);
}

void Subscribe_ZombieTurnStart(void(*function_pointer)(void), int priority) {
	InsertToArrayAt(&zombie_turn.start, function_pointer, priority);
}

void Subscribe_ZombieTurnUpdate(void(*function_pointer)(void), int priority) {
	InsertToArrayAt(&zombie_turn.update, function_pointer, priority);
}

void Subscribe_ZombieTurnEnd(void(*function_pointer)(void), int priority) {
	InsertToArrayAt(&zombie_turn.end, function_pointer, priority);
}

/*______________________________________________________________
@brief It is important this function is called when the game level exits!!
*/
void UnsubscribeAllEvents(void) {
	int total_events_subscribed;
	
	// Remove references to all events to player start, if any
	if (total_events_subscribed = player_turn.start.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			player_turn.start.events[index].event = NULL;
		}
	}
	// Remove references to all events to player update, if any
	if (total_events_subscribed = player_turn.update.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			player_turn.update.events[index].event = NULL;
		}
	}
	// Remove references to all events to player end, if any
	if (total_events_subscribed = player_turn.end.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			player_turn.end.events[index].event = NULL;
		}
	}
	// Remove references to all events to zombie start, if any
	if (total_events_subscribed = zombie_turn.start.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			zombie_turn.start.events[index].event = NULL;
		}
	}
	// Remove references to all events to zombie update, if any
	if (total_events_subscribed = zombie_turn.update.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			zombie_turn.update.events[index].event = NULL;
		}
	}
	// Remove references to all events to zombie end, if any
	if (total_events_subscribed = zombie_turn.end.count) {
		for (int index = 0; index < total_events_subscribed; ++index) {
			zombie_turn.end.events[index].event = NULL;
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

//______________________________________________________________
// Array Manipulation Functions

void InsertToArrayAt(TurnEvent *turn_array, void(*function_pointer)(void), int priority) {
	// Check with functions has a priority lower than the new one, we will ignore those
	int insert_index = 0, max = turn_array->count;

	for (int index = 0; index < max; ++index) {
		if (turn_array->events->priority > priority) {
			insert_index = index;
			break;
		}
	}

	// Shift all functions with lower priority backwards to make space for the new function
	for (int index = max; index > insert_index; --index) {
		turn_array->events[index + 1] = turn_array->events[index];
	}
	// Finally, insert the new function
	turn_array->events[insert_index].event = function_pointer;
	turn_array->events[insert_index].priority = priority;

	// Increase the count to match new size
	++turn_array->count;
}
