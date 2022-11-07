//Header files here
#include "Grid.h"
#include "Particles.h"
#include "WaveSystem.h"
#include "Screenshake.h"
#include "Hearts.h"
#include "SoundManager.h"

#include "GameLoop.h"
#include "TManager.h"
#include "TPlayer.h"

void gameLevelInit(void){
    grid_init();
    InitWaveSystem();
    InitializeLife();
    InitSoundManager(GAMEBGM);
	TManagerInit(DEFAULT_SPAWN_SEED);
	TPlayerInit();

	// Initialize game loop last, after all the events have been subscribed
	GameLoopInit();
}
void gameLevelUpdate(void){
    UpdateCameraShaker();

    GameLoopUpdate();
    //MAIN GAME LOOP GOES HERE
    grid_update();
    UpdateWave();




    //DRAWING AND UPDATING OF GRID

    //UPDATE HEARTS
    UpdateLife();

    //UPDATE VFX
    UpdateEffects();
	RenderHand();
    //CLEAR SCREEN
    CP_Graphics_ClearBackground(BLACK);
}
void gameLevelExit(void){
    UnsubscribeAllEvents();
    ClearHearts();
    KillSoundManager();
}