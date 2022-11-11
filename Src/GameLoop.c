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
void RemoveFromArrayAt(TurnEvent* turn_array, void(*function_pointer)(void));
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
// Subscription/Unsubscription events

/*______________________________________________________________
@brief 
*/
void SubscribeEvent(TurnEventType event_type, void(*function_pointer)(void), int priority) {
	TurnEvent *chosen_event = NULL;
	switch (event_type) {
	case PLAYER_START:  chosen_event = &player_turn.start; break;
	case PLAYER_UPDATE: chosen_event = &player_turn.update; break;
	case PLAYER_END:    chosen_event = &player_turn.end; break;

	case ZOMBIE_START:  chosen_event = &zombie_turn.start; break;
	case ZOMBIE_UPDATE: chosen_event = &zombie_turn.update; break;
	case ZOMBIE_END:    chosen_event = &zombie_turn.end;  break;
	}

	if (chosen_event == NULL) return;
	InsertToArrayAt(chosen_event, function_pointer, priority);
}

void UnsubscribeEvent(TurnEventType event_type, void(*function_pointer)(void)) {
	TurnEvent *chosen_event = NULL;
	switch (event_type) {
	case PLAYER_START:  chosen_event = &player_turn.start; break;
	case PLAYER_UPDATE: chosen_event = &player_turn.update; break;
	case PLAYER_END:    chosen_event = &player_turn.end; break;

	case ZOMBIE_START:  chosen_event = &zombie_turn.start; break;
	case ZOMBIE_UPDATE: chosen_event = &zombie_turn.update; break;
	case ZOMBIE_END:    chosen_event = &zombie_turn.end;  break;
	}

	if (chosen_event == NULL) return;
	RemoveFromArrayAt(chosen_event, function_pointer);
}

/*______________________________________________________________
@brief Similar to UnsubscribeAllEvents but this forces ALL events to be zeroed out, even the NULL ones
*/
void ZeroOutAllEvents(void) {
	// Remove references to all events to player start
	for (int index = 0; index < MAX_EVENTS_BUFFER; ++index) {
		player_turn.start.events[index].event = NULL;
		player_turn.start.events[index].priority = -9999;
	}
	// Remove references to all events to player update
	for (int index = 0; index < MAX_EVENTS_BUFFER; ++index) {
		player_turn.update.events[index].event = NULL;
		player_turn.update.events[index].priority = -9999;
	}
	// Remove references to all events to player end
	for (int index = 0; index < MAX_EVENTS_BUFFER; ++index) {
		player_turn.end.events[index].event = NULL;
		player_turn.end.events[index].priority = -9999;
	}
	// Remove references to all events to zombie start
	for (int index = 0; index < MAX_EVENTS_BUFFER; ++index) {
		zombie_turn.start.events[index].event = NULL;
		zombie_turn.start.events[index].priority = -9999;
	}
	// Remove references to all events to zombie update
	for (int index = 0; index < MAX_EVENTS_BUFFER; ++index) {
		zombie_turn.update.events[index].event = NULL;
		zombie_turn.update.events[index].priority = -9999;
	}
	// Remove references to all events to zombie end
	for (int index = 0; index < MAX_EVENTS_BUFFER; ++index) {
		zombie_turn.end.events[index].event = NULL;
		zombie_turn.end.events[index].priority = -9999;
	}

	// Reset the counters to 0 as well
	player_turn.start.count = 0;
	player_turn.update.count = 0;
	player_turn.end.count = 0;

	zombie_turn.start.count = 0;
	zombie_turn.update.count = 0;
	zombie_turn.end.count = 0;
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

	for (int index = 0; index <= max; ++index) {
		if (turn_array->events[index].priority < priority) {
			insert_index = index;
			break;
		}
	}

	// Shift all functions with lower priority backwards to make space for the new function
	for (int index = max; index >= insert_index; --index) {
		turn_array->events[index + 1] = turn_array->events[index];
	}
	// Finally, insert the new function
	turn_array->events[insert_index].event = function_pointer;
	turn_array->events[insert_index].priority = priority;

	// Increase the count to match new size
	++turn_array->count;
}

void RemoveFromArrayAt(TurnEvent *turn_array, void(*function_pointer)(void)) {
	int remove_index = -1, max = turn_array->count;

	for (int index = 0; index < max; ++index) {
		if (turn_array->events[index].event == function_pointer) {
			remove_index = index;
			break;
		}
	}

	// If function not found, stop
	if (remove_index < 0) return;

	// Else, shift all the functions forward
	for (int index = remove_index; index < max; ++index) {
		turn_array->events[index] = turn_array->events[index + 1];
	}

	// Decrease the count to match new size
	--turn_array->count;
}
