#include <stdio.h>

#include "gamelevel.h"
#include "ColorTable.h"

#include "GameLoop.h"
#include "TManager.h"
#include "TPlayer.h"

void TTestInit(void) {
	/* ISTG do not fullscreen the game while debugging, serious, 
	   everytime the game breaks and it fking pauses making me stuck in fullscreen
	   and requiring me to restart my com, a baby dies. The blood is on your
	   hands CProcessing, I hope you are proud of yourself. */
	CP_System_SetWindowSize(1920, 1080); 

	TManagerInit(DEFAULT_SPAWN_SEED);
	TPlayerInit();

	// Initialize game loop last, after all the events have been subscribed
	GameLoopInit();
}

int Render(void) {
	CP_Graphics_ClearBackground(BLACK);
	RenderHand();
}

void TTestUpdate(void) {
	GameLoopUpdate();
	Render();
}

void TTestExit(void) {
	UnsubscribeAllEvents();
}

void Jarrett() {
	CP_Engine_SetNextGameState(TTestInit, TTestUpdate, TTestExit);
	GameLoopInit();
}
