/*!
@file	  GameLoop.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@date     22/11/2022
@brief    This source file contains event arrays for different events in game, such as player start, zombie update, etc. Functions from
		  elsewhere in the solution can subscribe to these events and will be called when it happens. There are 9 functions in this file,

		  GameLoopInit - Called at the start of the game level to handle the turns between the player and the zombies.
		  GameLoopSwitch - Switches the turn state, calls the end events for the current state and the start events for the new one.
		  GameLoopUpdate - Every update, calls all functions subscribed to the current turn's update.

		  SubscribeEvent - Add a function to one of the events array, should the event happen, the function will be called.
		  UnsubscribeEvent - Remove a function from one of the event arrays.
		  UnsubscribeAllEvents - Resets ALL events subscribed to zero/null.
		  ZeroOutAllEvents - Similar to UnsubscribeAllEvents but this forces ALL events to be zeroed out, even the NULL ones.
		  
		  InsertToArrayAt - Inserts a new function pointer into the events array.
		  RemoveFromArrayAt - Removes an existing function pointer from the events array.
________________________________________________________________________________________________________*/

#include "GameLoop.h"

TurnType current_turn_type;	// Enum to show current turn event type (Start, Update, End)
Turn *current_turn;			// Pointer to the current turn events (Start, Update, End)

//______________________________________________________________
// Event subscriptions arrays
Turn player_turn;	// Player turn events (Start, Update, End)
Turn zombie_turn;	// Zombie turn events (Start, Update, End)

#pragma region
void InsertToArrayAt(TurnEvent* turn_array, void(*function_pointer)(void), int priority);
void RemoveFromArrayAt(TurnEvent* turn_array, void(*function_pointer)(void));
#pragma endregion Forward Declarations

//______________________________________________________________
// All "public" functions (Basically those in the GameLoop.h)

/*______________________________________________________________
@brief Needs to be called at the start of the game level to handle the turns between the player and the zombies
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

@param[in] new_turn - The turn type to swap to next
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
	for (int index = 0; index < current_turn->update.count; ++index) {
		current_turn->update.events[index].event();
	}
}

//______________________________________________________________
// Subscription/Unsubscription events

/*______________________________________________________________
@brief Add a function to one of the events array, should the event happen, the function will be called.

@param[in] event_type - The type of event to subscribe to
@param[in] function_pointer - The function to add to the event array
@param[in] priority - If the function should be called first, higher priority functions get called first
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

/*______________________________________________________________
@brief Remove a function from one of the event arrays.

@param[in] event_type - The type of event to subscribe to
@param[in] function_pointer - The pointer to the function to subscribe
*/
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
@brief Similar to UnsubscribeAllEvents but this forces ALL events to be zeroed out, even the NULL ones.
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
@brief Resets ALL events subscribed to zero/null, it is important this function is called when the game level exits!!
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

/*______________________________________________________________
@brief Inserts a new function pointer into the events array,
	   shifts all the functions below the new function down by 1

@param[in] turn_array - The array of events to add the new function pointer to
@param[in] function_pointer - The new function to add
@param[in] priority - Where in the array should the function pointer be placed in
*/
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

/*______________________________________________________________
@brief Removes an existing function pointer from the events array,
	   shifts all the functions below the removed functions up by 1

@param[in] turn_array - The array of events to remove the function pointer from
@param[in] function_pointer - The function to remove
*/
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
