#include <stdio.h>

#include "gamelevel.h"
#include "ColorTable.h"

#include "GameLoop.h"
#include "TManager.h"
#include "TPlayer.h"

void TTestInit(void) {
	CP_System_SetWindowSize(1920, 1080);
	GameLoopInit();
	TManagerInit(DEFAULT_SPAWN_SEED);
	TPlayerInit();
}

int Render(void) {
	CP_Graphics_ClearBackground(BLACK);
	RenderHand();
}

void TTestUpdate(void) {
	GameLoopUpdate();

	Render();
}

void Jarrett() {
	CP_Engine_SetNextGameState(TTestInit, TTestUpdate, NULL);
	GameLoopInit();
}
