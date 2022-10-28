#include "GameLoop.h"
#include "gamelevel.h"

void Jarrett() {
	CP_Engine_SetNextGameState(gameLevelInit, gameLevelUpdate, gameLevelExit);
	GameLoopInit();
}